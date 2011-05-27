#include "World.h"


World::World(void)
{
	p_activeCamera	= NULL;
	p_activeLight	= NULL;
	p_skybox		= NULL;
	p_terrain		= NULL;
	p_fog			= NULL;
	p_underWaterFog	= NULL;
	p_grass_prototype= NULL;
	p_grass_growth	= NULL;
	p_tree1_prototype= NULL;
	p_tree1_growth	= NULL;
	p_tree2_prototype= NULL;
	p_tree2_growth	= NULL;
	p_water			= NULL;
}


World::~World(void)
{
	SAFE_DELETE_PTR(p_activeCamera);
	SAFE_DELETE_PTR(p_activeLight);
	SAFE_DELETE_PTR(p_skybox);
	SAFE_DELETE_PTR(p_terrain);
	SAFE_DELETE_PTR(p_fog);
	SAFE_DELETE_PTR(p_underWaterFog);
	SAFE_DELETE_PTR(p_grass_prototype);
	SAFE_DELETE_PTR(p_grass_growth);
	SAFE_DELETE_PTR(p_tree1_prototype);
	SAFE_DELETE_PTR(p_tree1_growth);
	SAFE_DELETE_PTR(p_tree2_prototype);
	SAFE_DELETE_PTR(p_tree2_growth);
	SAFE_DELETE_PTR(p_water);
	textureManager.~TextureManager();
	shaderManager.~ShaderManager();
	printf("WORLD deleted.......\n"); 
}

void World::draw()
{
	// preprocess - one pass for LOD & occlusion queries



	// 1st pass (light shadows - light map...)
	p_activeLight->beginShadowMap();
		// render whole shadow casting&recieving scene
		

		p_terrain->draw();
		glEnable(GL_BLEND);
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		p_grass_growth->draw();
		p_tree1_growth->draw();
		p_tree2_growth->draw();
		glDisable(GL_BLEND);

		// draw other models...

		// TODO

	p_activeLight->endShadowMap();

	// 2nd pass (water)
	p_activeCamera->shoot();
	p_water->activeCamera = p_activeCamera;
	// WATER REFLECTION RENDER
	p_water->beginReflection();
		drawReflection();
	p_water->endReflection();
	// WATER REFRACTION RENDER
	p_water->beginRefraction();
		drawUnderWater();
	p_water->endRefraction();
	//p_water

	


	// 3rd pass (assembly)
	//p_fog->turnOn(); 
	if (g_godraysEnabled){p_godRays->begin();}

	if (p_activeCamera->getPosition().y>=WATER_HEIGHT-1.0){
		//p_fog->turnOn();
		p_terrain->drawOverWater();
		//p_fog->turnOff();
	}
	if (p_activeCamera->getPosition().y<=WATER_HEIGHT+1.0){
		drawUnderWater();
	}
	p_skybox->draw();
	p_water->draw(); // draw water surface
	
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	p_grass_growth->draw();
	p_tree1_growth->draw();
	p_tree2_growth->draw();
	glDisable(GL_BLEND);
	
	if (g_godraysEnabled){
		v3 lightDir = (p_activeLight->position - p_activeCamera->getPosition()).getNormalized();
		p_godRays->lightDirDOTviewDirValue = lightDir.dot(p_activeCamera->getDirection());
		p_godRays->end();
	}
	//box->draw();
	// draw other models...
		// TODO
	if(g_showTextures){
		p_water->showTextures();
		p_activeLight->showTextures();
	}
	//p_terrain->drawNormals();
	//p_fog->turnOff();
	
}

void World::drawUnderWater(){
	//p_underWaterFog->turnOn();
	p_terrain->drawUnderWater();
	//p_underWaterFog->turnOff();
}
void World::drawReflection(){
	g_drawingReflection = true;
	p_skybox->draw();
	p_terrain->cut = true;
	p_terrain->flip= true;
	//double plane[4] = {0.0, 1.0, 0.0, -WATER_HEIGHT+0.5};
	//glEnable(GL_CLIP_PLANE0);
	//glClipPlane(GL_CLIP_PLANE0, plane);
	p_terrain->drawOverWater();
	//glDisable(GL_CLIP_PLANE0);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	p_grass_growth->draw();
	p_tree1_growth->draw();
	p_tree2_growth->draw();
	glDisable(GL_BLEND);
	g_drawingReflection = false;
}

void World::windowSizeChanged(int width, int height)
{
	p_water->windowSizeChanged(width, height);
	p_godRays->windowSizeChanged(width, height);
}

void World::drawForLOD(){}

void World::init()
{
	printf("INITIALIZING WORLD:\n");
	
	p_fog = new Fog(0.01f, 50.f, 500.f, v4(HORIZON_COLOR));
	p_fog->init();
	p_underWaterFog = new Fog(0.01f, 10.f, 200.f, v4(WATER_DEPTH_COLOR));
	p_underWaterFog->init();
	
	p_terrain = new Terrain(&textureManager,&shaderManager);
	p_terrain->init();

	//box = new BBox(v3(0,0,0),v3(10,10,10),v3(1,1,1));
	
	p_activeCamera = new Camera();
	//p_activeCamera->setup(v3(-100.0,60.f,60.f), v3(1.0,-0.7f,-0.4f), v3(0.0,1.f,0.f), &g_WinWidth, &g_WinHeight, 60.0, 1.f, 1000.f);
	p_activeCamera->setup(LIGHT_POSITION, -LIGHT_POSITION, v3(0.0,1.f,0.f), &g_WinWidth, &g_WinHeight, 60.0, 1.f, 1000.f);
	p_activeCamera->setTerrain(p_terrain);
	p_activeCamera->setMode(g_cameraMode);

	// sun
	p_activeLight = new Light();
	p_activeLight->init(); // setup framebuffers for shadow mapping
	p_activeLight->setup(GL_LIGHT0, g_light_position, LIGHT_DIRECTION, sunAmb, sunDif, sunSpe, 180, 0.0);
	p_activeLight->turnOn();

	// sky
	p_skybox = new SkyBox(&textureManager, &shaderManager, SKYBOX_TEX_FILENAMES);
	p_skybox->init();
	p_skybox->p_activeCamera = p_activeCamera;
	p_skybox->p_light = p_activeLight;

	


	p_water = new WaterSurface(&textureManager, &shaderManager);
	p_water->init();  // create FBOs...



	p_godRays = new GodRays(&shaderManager, p_activeLight);
	
	int count = 0;
	// plant grass	
	printf("Planting grass...\n");
	p_grass_prototype = new Grass(&textureManager,&shaderManager);
	p_grass_prototype->init();
	p_grass_growth = p_grass_prototype->getCopy();
	grass_planter.init(	p_terrain,
						p_grass_prototype,
						p_grass_growth,
						GRASS_MIN_HEIGHT,
						GRASS_MAX_HEIGHT,
						GRASS_MIN_DIST,
						500,
						500 );
	count = grass_planter.plantVegetationCount(g_GrassCount);
	printf("count: %i\n", count);

	// plant trees
	printf("Planting trees1...\n");
	p_tree1_prototype = new Tree1(&textureManager,&shaderManager);
	p_tree1_prototype->init();
	p_tree1_growth = p_tree1_prototype->getCopy();
	tree1_planter.init(	p_terrain,
						p_tree1_prototype,
						p_tree1_growth,
						TREE1_MIN_HEIGHT,
						TREE1_MAX_HEIGHT,
						TREE1_MIN_DIST,
						100,
						100 );
 	count = tree1_planter.plantVegetationCount(g_Tree1Count);
	printf("count: %i\n", count);

	// plant trees
	printf("Planting trees2...\n");
	p_tree2_prototype = new Tree2(&textureManager,&shaderManager);
	p_tree2_prototype->init();
	p_tree2_growth = p_tree2_prototype->getCopy();
	tree2_planter.init(	p_terrain,
						p_tree2_prototype,
						p_tree2_growth,
						TREE2_MIN_HEIGHT,
						TREE2_MAX_HEIGHT,
						TREE2_MIN_DIST,
						100,
						100 );
 	count = tree2_planter.plantVegetationCount(g_Tree2Count);
	printf("count: %i\n", count);


	printf("WORLD CREATED:\n");

}
void World::update(double i_time)
{
	p_activeCamera->setMode(g_cameraMode);
	p_activeCamera->update(i_time);
	//g_time = i_time;
	/*int modelCnt = v_models.size();
	for (int i=0; i<modelCnt; i++){
		v_models[i]->update(time);
	}
	p_skybox->update(time);
	p_terrain->update(time);
	p_activeLight->update(time);
	p_grass->update(time);*/
}

void World::translate(v3 &movVector)
{

}


void World::rotate(v3 &axis, float angleRad)
{

}


void World::scale(v3 &scaleVector)
{

}


