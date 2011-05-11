#ifndef _TREE2_H
#define _TREE2_H

#include "Vegetation.h"

class Tree2 :
	public Vegetation
{
public:
	Tree2(TextureManager *texManager, ShaderManager *shManager);
	Tree2(Tree2* copy);
	~Tree2(void);

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
