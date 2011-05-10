#ifndef _WATERSURFACE_H
#define _WATERSURFACE_H

#include "scenemodel.h"
#include "utils.h"
#include "Camera.h"

class WaterSurface :
	public SceneModel
{
public:
	WaterSurface(TextureManager *texManager, ShaderManager *shManager);
	~WaterSurface(void);

	void draw();

	void init();

	void update(double time);

	void translate(v3 &movVector);

	void rotate(v3 &axis, float angleRad);

	void scale(v3 &scaleVector);

	void windowSizeChanged(int width, int height);
	void beginReflection();
	void endReflection();
	void beginRefraction();
	void endRefraction();


	void showTextures();
	GLuint fb_refl_ID;
	GLuint cb_refl_ID;
	GLuint db_refl_ID;
	GLuint fb_refr_ID;
	GLuint cb_refr_ID;
	GLuint db_refr_ID;
	Shader* shader;
	int		water_reflection_loc;
	int		water_refraction_loc;
	int		water_depth_loc;
	v4		viewPos;
	v4		lightPos;
	v4		waterColor;
	v4		waterDepth;
	Camera *activeCamera;
};

#endif

