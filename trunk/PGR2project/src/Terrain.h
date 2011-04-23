#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "SceneModel.h"
#include "../common/png.h"
#include "settings.h"
#include "utils.h"

class Terrain :
	public SceneModel
{
public:
	Terrain(TextureManager *texManager);
	~Terrain(void);

	void draw();
	
	void drawNormals();


	void init();

	void update(float time);

	void translate(v3 &movVector);

	void rotate(v3 &axis, float angleRad);

	void scale(v3 &scaleVector);

	void loadHeightMap(string filename);
	void loadTextures(string filename, int count);

	float getHeightAt(float x, float y);
	float getHeightAt(int x, int y);
// attributes:

	float*	heightMap;
	int		dim_x, dim_y;
	int		hdim_x, hdim_y;
	float	sz_x, sz_y;
	int		reduction;
	GLuint	drawingMethod;
	GLuint	vboId;
	GLuint	vboCount;
	GLuint  eboId;
	GLuint  eboCount;
	GLfloat * vertices;
	GLfloat * normals ;
	GLfloat * texCoords;
	GLuint  * elements;

	GLint offsets	[VBO_ATR_COUNT];
	GLint sizes		[VBO_ATR_COUNT];
	GLint channels	[VBO_ATR_COUNT];
	GLint typeSizes	[VBO_ATR_COUNT];
	GLint glTypes	[VBO_ATR_COUNT];

	vector<int>		textureIds;

	GLint	shaderID;

};

#endif

