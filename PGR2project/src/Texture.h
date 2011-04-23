#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "settings.h"
#include "../common/png.h"

class Texture
{
public:
	Texture(void);
	~Texture(void);
	
	void load(string filename);

	void bind(GLenum texUnit);
	void unbind();

	GLint	format;
	GLenum	textureUnit;
	GLint	textureUnitNumber;
	GLuint   id;
	GLsizei width;
	GLsizei height;
	string  inShaderName;

	static GLint texUnitToNumber(GLenum texUnit){
		return texUnit - GL_TEXTURE0;
	}
};

#endif