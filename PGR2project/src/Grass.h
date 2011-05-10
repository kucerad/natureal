#ifndef _GRASS_H
#define _GRASS_H

#include "Vegetation.h"

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

	void update(double time);

	void translate(v3 &movVector);

	void rotate(v3 &axis, float angleRad);

	void scale(v3 &scaleVector);
	
	void bakeToVBO();
	
	v3	 transformTexCoords(v3 &origTexCoords);
	void fixTexType();

	int			texPart_x;
	int			texPart_y;
	int			textureId;
	int			shaderId;
	Shader*		shader;
	int			textureWaveId;
	GLuint		vboId;
	
	
};

#endif
