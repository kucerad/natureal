#ifndef _GRASS_H
#define _GRASS_H

#include "Vegetation.h"

const GLfloat CROSS_VERTEX_ARRAY[] =
{
	 -0.5f,0.0f,0.0f, -0.5f,1.0f,0.0f, 0.5f,1.0f,0.0f, 0.5f,0.0f,0.0f,
	 0.0f,0.0f,-0.5f,  0.0f,1.0f,-0.5, 0.0f,1.0f,0.5f, 0.0f,0.0f,0.5f,
	 0.0, 1.0, 0.0,    0.0, 1.0, 0.0,   0.0, 1.0, 0.0,  0.0, 1.0, 0.0,
	 0.0, 1.0, 0.0,    0.0, 1.0, 0.0,   0.0, 1.0, 0.0,  0.0, 1.0, 0.0,
	 0.0, 0.0,         0.0, 1.0,	   1.0, 1.0, 	   1.0, 0.0,
	 0.0, 0.0,         0.0, 1.0,	   1.0, 1.0, 	   1.0, 0.0
};
const GLint cross_vertexCount	 = 8;
const GLint cross_NormalOffset   = cross_vertexCount*3*sizeof(GLfloat);
const GLint cross_TexCoordOffset = cross_vertexCount*3*2*sizeof(GLfloat);



class Grass :
	public Vegetation
{
public:
	Grass(TextureManager *texManager, ShaderManager *shManager);
	Grass(Grass* copy);
	~Grass(void);

	Vegetation* getCopy();

	void draw();

	void init();

	void update(float time);

	void translate(v3 &movVector);

	void rotate(v3 &axis, float angleRad);

	void scale(v3 &scaleVector);
	
	void bakeToVBO();
	
	int			textureId;
	int			shaderId;
	Shader*		shader;
	int			textureWaveId;
	GLuint		vboId;
	
	
};

#endif
