#include "Texture.h"


Texture::Texture(void)
{
	id = 0;

}


Texture::~Texture(void)
{

}


void Texture::load(string filename)
{
	// load image
	PNG png;
	png.load(filename);
	width	= png.width;
	height	= png.height;
	unsigned char * data = png.getData();
	// generate
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	png.clear();
}

void Texture::bind(GLenum texUnit)
{
	textureUnit = texUnit;
	textureUnitNumber = texUnitToNumber(texUnit);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(texUnit);
	glClientActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind()
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}