#ifndef _WORLD_H
#define _WORLD_H

#include "globals.h"
#include "utils.h"
#include "scenemodel.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "BBox.h"
#include "Camera.h"
#include "Light.h"
#include "Fog.h"
#include "TextureManager.h"
#include "Grass.h"
#include "Tree1.h"
#include "Tree2.h"
#include "Planter.h"
#include "WaterSurface.h"
#include "GodRays.h"
#include "pgr2model.h"
#include "LODmodel.h"
#include "TestModel.h"

class World
{
public:
	World(void);
	~World(void);
	void draw();
	void drawUnderWater();
	void drawReflection();
	void drawForLOD();
	void windowSizeChanged(int width, int height);

	void startLODBuffer();
	void endLODBuffer();

	void init();

	void initModels();
	void drawModels();

	void deinitModels();

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
	GodRays				*p_godRays;
	Fog					*p_fog;
	Fog					*p_underWaterFog;
	WaterSurface		*p_water;

	Grass				*p_grass_prototype;
	Vegetation			*p_grass_growth;
	Tree1				*p_tree1_prototype;
	Vegetation			*p_tree1_growth;
	Tree2				*p_tree2_prototype;
	Vegetation			*p_tree2_growth;

	PGR2Model			*p_model1;
	TestModel			*p_testModel;

	TextureManager		textureManager;
	ShaderManager		shaderManager;

	// planters
	Planter				grass_planter;
	Planter				tree1_planter;
	Planter				tree2_planter;

private:
	// models:
	LODmodel			*house1;
	LODmodel			*house2;
	LODmodel			*house3;
	LODmodel			*bridge;
	LODmodel			*tower1;
	LODmodel			*tower2;
	LODmodel			*eggbox;
	LODmodel			*haywagon;
	LODmodel			*well;

	GLuint				fb_LOD_ID;
	GLuint				cb_LOD_ID;
	GLuint				db_LOD_ID;

	void show_textures();
	void drawForShadowmapping();
};

#endif
