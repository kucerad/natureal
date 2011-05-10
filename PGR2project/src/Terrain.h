#ifndef _TERRAIN_H
#define _TERRAIN_H

#include "SceneModel.h"
#include "../common/png.h"
#include "settings.h"
#include "utils.h"
#include "Shader.h"

class Terrain :
	public SceneModel
{
public:
	Terrain(TextureManager *texManager, ShaderManager *shManager);
	~Terrain(void);

	void draw();
	void drawUnderWater();

	void drawNormals();


	void init();

	void update(double time);

	void translate(v3 &movVector);

	void rotate(v3 &axis, float angleRad);

	void scale(v3 &scaleVector);

	void loadHeightMap(string filename, int res_x, int res_y);
	void loadTextures(string filename, int count);

	
	float getHeightAt(float x, float y);
	float getHeightAt(int x, int y);
// attributes:

	float*	heightMap;
	int		dim_x, dim_y;
	float   red_x, red_y;
	float	sz_x, sz_y;
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

	Shader *	shader;
	bool		cut;
	bool		flip;
private:
	float getHeightAt(float *map, int res_x, int res_y, float x, float y);
	float getHeightAt(float *map, int res_x, int res_y, int x, int y);

	int border_values_location;
	int border_widths_location;
	int heightInterval_location;
};

#endif

