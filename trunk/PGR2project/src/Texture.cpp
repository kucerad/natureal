#include "Texture.h"


Texture::Texture(void)
{
	id = 0;
}


Texture::~Texture(void)
{

}


void Texture::load(string filename, GLint unitNumber, bool buildMipmaps, GLint wrapMode, GLint filterMode)
{
	// pre-set texture unit
	textureUnit = GL_TEXTURE0 + unitNumber;
	textureUnitNumber = unitNumber;
	// load image
	PNG png;
	png.load(filename);
	width	= png.width;
	height	= png.height;
	unsigned char * data = png.getData();
	// generate
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	if (buildMipmaps){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	png.clear();
}

void Texture::bind(GLenum texUnit)
{
	textureUnit = texUnit;
	textureUnitNumber = texUnitToNumber(texUnit);
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(texUnit);
	//glClientActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind()
{
	glActiveTexture(textureUnit);
	//glClientActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}