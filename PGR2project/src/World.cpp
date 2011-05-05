#include "World.h"


World::World(void)
{
	p_activeCamera	= NULL;
	p_activeLight	= NULL;
	p_skybox		= NULL;
	p_terrain		= NULL;
	p_fog			= NULL;
	p_grass_prototype= NULL;
	p_grass_growth	= NULL;
	p_water			= NULL;
}


World::~World(void)
{
	SAFE_DELETE_PTR(p_activeCamera);
	SAFE_DELETE_PTR(p_activeLight);
	SAFE_DELETE_PTR(p_skybox);
	SAFE_DELETE_PTR(p_terrain);
	SAFE_DELETE_PTR(p_fog);
	SAFE_DELETE_PTR(p_grass_prototype);
	SAFE_DELETE_PTR(p_grass_growth);
	SAFE_DELETE_PTR(p_water);
	textureManager.~TextureManager();
	shaderManager.~ShaderManager();
	printf("WORLD deleted.......\n"); 
}

void World::draw()
{
	// 1st pass (water)
	p_activeCamera->shoot();
	p_water->activeCamera = p_activeCamera;
	// WATER REFLECTION RENDER
	p_water->beginReflection();
		drawReflection();
	p_water->endReflection();
	// WATER REFRACTION RENDER
	//p_water

	// 2nd pass (other...)
	//p_fog->turnOn();
	//glPushMatrix();
	//glScalef(1.f, -1.f, 1.f);
	p_terrain->draw();
	p_skybox->draw();
	p_water->draw();
	//glPopMatrix();
	box->draw();
	
	
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	p_grass_growth->draw();	
	glDisable(GL_BLEND);

	
	p_water->showTexture(0,0,200,200);
	
	//p_terrain->drawNormals();
	//p_fog->turnOff();
}

void World::drawUnderWater(){}
void World::drawReflection(){
	p_skybox->draw();
	p_terrain->cut = true;
	p_terrain->flip= true;
	p_terrain->draw();
}

void World::windowSizeChanged(int width, int height)
{
	p_water->windowSizeChanged(width, height);
}

void World::drawForLOD(){}

void World::init()
{
	printf("INITIALIZING WORLD:\n");
	
	p_fog = new Fog();
	p_fog->init();
	
	p_terrain = new Terrain(&textureManager,&shaderManager);
	p_terrain->init();

	box = new BBox(v3(0,0,0),v3(10,10,10),v3(1,1,1));

	p_activeCamera = new Camera();
	p_activeCamera->setup(v3(0.0,0.f,0.f), v3(0.0,0.f,-1.f), v3(0.0,1.f,0.f), &g_WinWidth, &g_WinHeight, 60.0, 1.f, 1000.f);
	p_activeCamera->setTerrain(p_terrain);
	p_activeCamera->setMode(g_cameraMode);

	p_skybox = new SkyBox(&textureManager, &shaderManager, SKYBOX_TEX_FILENAMES);
	p_skybox->init();
	p_skybox->p_activeCamera = p_activeCamera;

	p_water = new WaterSurface(&textureManager, &shaderManager);
	p_water->init();  // create FBOs...


	p_grass_prototype = new Grass(&textureManager,&shaderManager);
	p_grass_prototype->init();

	p_activeLight = new Light();
	p_activeLight->setup(GL_LIGHT0, v3(0,10,0), v3(0,-1,0), sunAmb, sunDif, sunSpe, 180, 0.0);
	p_activeLight->turnOn();
	
	// plant grass
	Planter planter(p_terrain);
	p_grass_growth = p_grass_prototype->getCopy();
	planter.init(p_grass_prototype, p_grass_growth, GRASS_MIN_HEIGHT, GRASS_MAX_HEIGHT, GRASS_MIN_DIST, 100, 100 );
	planter.plantVegetationCount(g_GrassCount);
	

	// plant trees
	//planter.height_min = TREE1_MIN_HEIGHT;
	//planter.height_max = TREE1_MAX_HEIGHT;
	//planter.minDist = 3.f;
	//planter.res_x = 100;
	//planter.res_y = 100;
	//planter.desiredCount = 100;
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


