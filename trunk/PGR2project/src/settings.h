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
#include "../common/Matrix4x4.h"

#define BUFFER_OFFSET(i) ((char*) NULL + (i))

#define HEIGHTMAP_SOURCE "terrain/terrain2.png"
#define HEIGHTMAP_SCALE 0.001
#define HEIGHTMAP_INITHEIGHT -5

#define HUMAN_HEIGHT 5.f

static GLint    g_WinWidth             = 800;   // Window width
static GLint    g_WinHeight            = 600;   // Window height


using namespace std;

#endif