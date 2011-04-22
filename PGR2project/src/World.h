#ifndef _WORLD_H
#define _WORLD_H

#include "scenemodel.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "BBox.h"
#include "Camera.h"

class World :
	public SceneModel
{
public:
	World(void);
	~World(void);
	void draw();

	void init();

	void update(float time);

	void translate(v3 movVector);

	void rotate(v3 axis, float angleRad);

	void scale(v3 scaleVector);

// attributes:
	vector<SceneModel*> v_models;
	SkyBox				*p_skybox;
	Terrain				*p_terrain;
	BBox				*box;
	Camera				*p_activeCamera;

};

#endif
