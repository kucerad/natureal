#ifndef _TREE1_H
#define _TREE1_H

#include "Vegetation.h"





class Tree1 :
	public Vegetation
{
public:
	Tree1(TextureManager *texManager, ShaderManager *shManager);
	Tree1(Tree1* copy);
	~Tree1(void);

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
