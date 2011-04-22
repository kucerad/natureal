#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "SceneModel.h"
#include "../common/png.h"
#include "settings.h"

class Terrain :
	public SceneModel
{
public:
	Terrain(void);
	~Terrain(void);

	void draw();

	void init();

	void update(float time);

	void translate(v3 movVector);

	void rotate(v3 axis, float angleRad);

	void scale(v3 scaleVector);

	void loadHeightMap(string filename);

	float getHeightAt(float x, float y);
	float getHeightAt(int x, int y);
// attributes:

	float*	heightMap;
	int		dim_x, dim_y;
	float	sz_x, sz_y;
	GLuint	drawingMethod;
	GLuint	vboId;
	GLuint	vboCount;
	GLuint  eboId;
	GLuint  eboCount;

	GLint	shaderID;

};

#endif

