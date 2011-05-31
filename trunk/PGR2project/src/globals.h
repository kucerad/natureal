#ifndef _GLOBALS_H
#define _GLOBALS_H
#include "../common/GLEW/glew.h"
#include "../common/GL/glut.h"
#include "../common/timer.h"
#include "../common/Vector3.h"
#include "../common/Vector4.h"
#include "../common/Matrix4x4.h"

enum CameraMode{
	FREE,
	TERRAIN_RESTRICTED,
	TERRAIN_CONNECTED,
	WALK
};

struct Statistics{
	float	fps;
	int		primitives;
};

extern	GLint					g_WinWidth;   // Window width
extern	GLint					g_WinHeight;   // Window height
extern	double					g_time;
extern	bool					g_godraysEnabled;
extern	bool					g_fastMode;
extern	v3						g_light_position;
extern	bool					g_drawingReflection;
extern	bool					g_showTextures;

extern	bool					g_ShadowMappingEnabled;
extern	m4						g_LightMVPCameraVInverseMatrix;
extern	m4						g_LightMVCameraVInverseMatrix;
extern	m4						g_LightPMatrix;



extern	CameraMode				g_cameraMode;

extern	Statistics				g_Statistics;
extern	v4						g_terrain_border_values;
extern	v4						g_terrain_border_widths;

extern int						g_GrassCount;
extern int						g_Tree1Count;
extern int						g_Tree2Count;
#endif