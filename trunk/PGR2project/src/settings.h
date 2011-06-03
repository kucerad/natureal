#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include "../common/GLEW/glew.h"
#include "../common/GL/glut.h"
#include "math.h"
#include "../common/Vector2.h"
#include "../common/Vector3.h"
#include "../common/Vector4.h"
#include "../common/Matrix4x4.h"
#include "globals.h"

#define BUFFER_OFFSET(i) ((char*) NULL + (i))

#define TIME_STEP 0.1f
#define TIME_QUERY_EXTENSION	"GL_EXT_timer_query"


#define HEIGHTMAP_SOURCE		"textures/terrain/terrain2.png"
#define HEIGHTMAP_SCALE			0.001
#define HEIGHTMAP_INITHEIGHT	5

#define TERRAIN_TEX_COUNT		5
#define TERRAIN_SIZE_X			500.f
#define TERRAIN_SIZE_Y			500.f
#define TERRAIN_RESOLUTION_X	100
#define TERRAIN_RESOLUTION_Y	100
#define TERRAIN_TEX_NAME		"textures/terrain/terrain_tex_%02i.png"
#define TERRAIN_VS_FILENAME		"shaders/terrain/terrain_vs.glsl"
#define TERRAIN_FS_FILENAME		"shaders/terrain/terrain_fs.glsl"
#define TERRAIN_UW_FS_FILENAME  "shaders/terrain/terrain_uw_fs.glsl"
#define TERRAIN_UW_VS_FILENAME  "shaders/terrain/terrain_uw_vs.glsl"
#define HORIZON_COLOR			v4(0.7f, 0.0f, 0.0f, 1.0f)




#define GRASS_COUNT				200
//#define GRASS_TEX_NAME			"textures/grass/grass_01.png"
#define GRASS_TEX_NAME			"textures/grass/grass_multitextureVert.png"
#define GRASS_WAVE_TEX_NAME		"textures/grass/dudv_02.png"
#define GRASS_VS_FILENAME		"shaders/grass/grass_vs.glsl"
#define GRASS_FS_FILENAME		"shaders/grass/grass_fs.glsl"
#define GRASS_MIN_HEIGHT		1.f
#define GRASS_MAX_HEIGHT		5.f
#define GRASS_MIN_DIST			0.2f
#define GRASS_SCALE				2.f


#define TREE1_COUNT				50
#define TREE1_TEX_NAME			"textures/tree1/tree_windbreak.png"
#define TREE1_VS_FILENAME		"shaders/tree/tree_vs.glsl"
#define TREE1_FS_FILENAME		"shaders/tree/tree_fs.glsl"
#define TREE1_MIN_HEIGHT		7.f
#define TREE1_MAX_HEIGHT		13.f
#define TREE1_MIN_DIST			5.f
#define TREE1_SCALE				5.f


#define TREE2_COUNT				50
#define TREE2_TEX_NAME			"textures/tree2/pine.png"
#define TREE2_VS_FILENAME		"shaders/tree/tree_vs.glsl"
#define TREE2_FS_FILENAME		"shaders/tree/tree_fs.glsl"
#define TREE2_MIN_HEIGHT		12.f
#define TREE2_MAX_HEIGHT		20.f
#define TREE2_MIN_DIST			5.f
#define TREE2_SCALE				6.f



#define SKYBOX_TEX_FILENAMES	"textures/skybox/512/sahara_%s.png"
#define SKYBOX_SIZE				800

#define WATER_DEPTH_COLOR		v4(0.1, 0.3, 0.5, 1.0)
#define WATER_VS_FILENAME		"shaders/water/water_vs.glsl"
#define WATER_FS_FILENAME		"shaders/water/water_fs.glsl"
#define WATER_HEIGHT			0.f
#define WATER_DUDV_MAP			"textures/water/water_dudv.png"
#define WATER_NORMAL_MAP		"textures/water/water_normal.png"
//#define WATER_DUDV_MAP			"textures/water/water_dudv.png"
//#define WATER_DUDV_MAP			"textures/water/water_dudv.png"

#define HUMAN_HEIGHT			2.f
#define HUMAN_BREATH_FREQ		1.2f
#define HUMAN_BREATH_AMPL		0.15f
#define HUMAN_ACTIVITY_DECAY    0.992f
#define HUMAN_ACTIVITY_INCR	    0.01f
#define HUMAN_MIN_ACTIVITY	    0.1f
#define HUMAN_MAX_ACTIVITY	    1.3f
#define HUMAN_POSITION          v3(0.0, 10.0, 0.0)
#define HUMAN_DIRECTION			v3(0.0, 0.0, -1.0)

//#define MODEL1_FILENAME			"models/Walls&Towers/watch2.pgr2"
//#define MODEL1_FILENAME			"models/Walls&Towers/bridge.pgr2"
//#define MODEL1_FILENAME			"models/MedievalVillage/haywagon.pgr2"
#define MODEL1_FILENAME			"models/MedievalVillage/house4.pgr2"
//#define MODEL1_FILENAME			"models/MedievalVillage/egg_box.pgr2"



static enum Attribs{
	INDEX,
	VERTEX,
	NORMAL,
	TANGENT,
	BINORMAL,
	TEXCOORD0,
	TEXCOORD1,
	TEXCOORD2,
	TEXCOORD3,
	TEXCOORD4,
	TEXCOORD5,
	TEXCOORD6,
	TEXCOORD7,
	ATTRIB0,
	ATTRIB1,
	ATTRIB2,
	ATTRIB3,
	ATTRIB4,
	FACENORMAL,
	// add new items here (must be before VBO_ATTRIB_CNT!!!)
	

	VBO_ATR_COUNT
};



#define GODRAYS_VS_FILENAME		"shaders/godrays/godrays_vs.glsl"
#define GODRAYS_FS_FILENAME		"shaders/godrays/godrays_fs.glsl"

#define LIGHT_POSITION			v3(397.f, 142.f, -8.f) // specific for sahara skybox...
//#define LIGHT_POSITION			v3(10.f, 300.f, 0.f) 
#define LIGHT_DIRECTION			v3(0.f, -1.f, 0.f)
#define SHADOWMAP_RESOLUTION_X  2048
#define SHADOWMAP_RESOLUTION_Y  2048


static v4 sunAmb  = v4(0.9,0.9, 1.0, 1.0);
static v4 sunDif  = v4(1.0,1.0, 1.0, 1.0);
static v4 sunSpe  = v4(0.5,0.5, 0.6, 1.0);

static GLfloat material_amd[4] = {0.6f, 0.6f, 0.6f, 1.0f};
static GLfloat material_spe[4] = {.5f, .5f, .5f, 1.0f};




using namespace std;

#endif