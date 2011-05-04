#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include "settings.h"
#include "Texture.h"
#include "utils.h"

class TextureManager
{
public:
	TextureManager(void);
	~TextureManager(void);

	int			loadTexture(string filename,string inShaderName = "not_in_shader_texture", GLint unitNumber=0, bool buildMipmaps=false, GLint wrapMode=GL_REPEAT, GLint filterMode=GL_LINEAR);
	void		bindTexture(int texId, GLenum texUnit);
	void		unbindTexture(int texId);
	Texture *	getTexture(int texId);
private:
	vector<Texture*> textures;
};

#endif

