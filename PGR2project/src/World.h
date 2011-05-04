#ifndef _WORLD_H
#define _WORLD_H

#include "scenemodel.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "BBox.h"
#include "Camera.h"
#include "Light.h"
#include "Fog.h"
#include "TextureManager.h"
#include "Grass.h"
#include "Planter.h"
#include "WaterSurface.h"

class World
{
public:
	World(void);
	~World(void);
	void draw();
	void drawForWater();
	void drawForLOD();
	

	void init();

	void update(double time);

	void translate(v3 &movVector);

	void rotate(v3 &axis, float angleRad);

	void scale(v3 &scaleVector);

// attributes:
	vector<SceneModel*> v_models;
	SkyBox				*p_skybox;
	Terrain				*p_terrain;
	BBox				*box;
	Camera				*p_activeCamera;
	Light				*p_activeLight;
	Fog					*p_fog;
	WaterSurface		*p_water;

	Grass				*p_grass_prototype;
	Vegetation			*p_grass_growth;

	TextureManager		textureManager;
	ShaderManager		shaderManager;
	
};

#endif
