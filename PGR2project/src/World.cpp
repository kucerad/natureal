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
}

void World::draw()
{
	p_activeCamera->shoot();
	p_skybox->draw();
	p_fog->turnOn();
	p_terrain->draw();
	box->draw();
	p_grass_growth->draw();
	//p_terrain->drawNormals();
	p_fog->turnOff();
}

void World::init()
{
	printf("INITIALIZING WORLD:\n");
	
	p_fog = new Fog();
	p_fog->init();
	
	p_terrain = new Terrain(&textureManager);
	p_terrain->init();

	box = new BBox(v3(0,0,0),v3(10,10,10),v3(1,1,1));

	p_activeCamera = new Camera();
	p_activeCamera->setup(v3(0.0,0.f,0.f), v3(0.0,0.f,-1.f), v3(0.0,1.f,0.f), &g_WinWidth, &g_WinHeight, 60.0, 1.f, 1000.f);
	p_activeCamera->setTerrain(p_terrain);
	p_activeCamera->setMode(CameraMode::FREE);

	p_skybox = new SkyBox(&textureManager, SKYBOX_TEX_FILENAMES);
	p_skybox->init();
	p_skybox->p_activeCamera = p_activeCamera;

	p_grass_prototype = new Grass(&textureManager);
	p_grass_prototype->init();

	p_activeLight = new Light();
	p_activeLight->setup(GL_LIGHT0, v3(0,10,0), v3(0,-1,0), sunAmb, sunDif, sunSpe, 180, 0.0);
	p_activeLight->turnOn();
	
	printf("WORLD CREATED:\n");

	Planter planter(p_terrain);
	p_grass_growth = p_grass_prototype->getCopy();
	planter.plantVegetation(p_grass_prototype, p_grass_growth);
	p_grass_growth->bakeToVBO();

	
	system("PAUSE");

}
void World::update(float i_time)
{
	g_time = i_time;
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


