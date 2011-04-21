#include "World.h"


World::World(void)
{
}


World::~World(void)
{
}

void World::draw()
{
	box->draw();
}

void World::init()
{
	box = new BBox(v3(-1,-1,-1),v3(1,1,1),v3(1,0,0));
}
void World::update(float time)
{
	int modelCnt = v_models.size();
	for (int i=0; i<modelCnt; i++){
		v_models[i]->update(time);
	}
	p_skybox->update(time);
	p_terrain->update(time);
}

void World::translate(v3 movVector)
{

}


void World::rotate(v3 axis, float angleRad)
{

}


void World::scale(v3 scaleVector)
{

}


