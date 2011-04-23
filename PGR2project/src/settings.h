#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include "../common/GLEW/glew.h"
#include "../common/GL/glut.h"
#include "math.h"
#include "../common/Vector3.h"
#include "../common/Vector4.h"
#include "../common/Matrix4x4.h"

#define BUFFER_OFFSET(i) ((char*) NULL + (i))

#define HEIGHTMAP_SOURCE "terrain/terrain2.png"
#define HEIGHTMAP_SCALE 0.001
#define HEIGHTMAP_INITHEIGHT -5

#define TERRAIN_TEX_COUNT 1
#define TERRAIN_TEX_NAME "terrain/terrain_tex_%02i.png"

#define SKYBOX_TEX_FILENAMES "skybox/512/sahara_%s.png"
#define SKYBOX_SIZE 999;

#define HUMAN_HEIGHT 5.f

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
	// add new items here (must be before VBO_ATTRIB_CNT!!!)


	VBO_ATR_COUNT
};



static GLint    g_WinWidth             = 800;   // Window width
static GLint    g_WinHeight            = 600;   // Window height

static v4 sunAmb  = v4(0.05,0.05, 0.2, 1.0);
static v4 sunDif  = v4(1.0,1.0, 1.0, 1.0);
static v4 sunSpe  = v4(1.0,1.0, 0.9, 1.0);

static GLfloat material_amd[4] = {0.6f, 0.6f, 0.6f, 1.0f};
static GLfloat material_spe[4] = {0.3f, 0.3f, 0.3f, 1.0f};

using namespace std;

#endif