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
	int		house1_lod;
	int		house1_samples;
	int		house2_lod;
	int		house2_samples;
	int		bridge_lod;
	int		bridge_samples;
	int		tower1_lod;
	int		tower1_samples;
	int		tower2_lod;
	int		tower2_samples;
	int		eggbox_lod;
	int		eggbox_samples;
	int		haywagon_lod;
	int		haywagon_samples;
	int		well_lod;
	int		well_samples;
	//int		house_lod;
	//int		house_samples;


};
static int g_nula = 0;

extern	GLint					g_WinWidth;   // Window width
extern	GLint					g_WinHeight;   // Window height
extern	double					g_time;
extern	bool					g_godraysEnabled;
extern	bool					g_fastMode;
extern	v3						g_light_position;
extern	bool					g_drawingReflection;
extern	bool					g_showTextures;
extern  bool					g_ParallaxMappingEnabled;
extern  float					g_ParallaxScale;
extern  float					g_ParallaxBias;


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

extern int g_Bumpmaps	   ;
extern int g_Heightmaps	   ;
extern int g_Specularmaps  ;
extern int g_Alphamaps	   ;

#endif