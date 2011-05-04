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

	void beginReflection();
	void endReflection();
	void showTexture(int x, int y, int szx, int szy);
	GLuint fbID;
	GLuint cbID;
	GLuint dbID;
	Shader* shader;
	int		water_reflection_loc;
	v4		viewPos;
	v4		lightPos;
	v4		waterColor;
	v4		waterDepth;
	Camera *activeCamera;
};

#endif

