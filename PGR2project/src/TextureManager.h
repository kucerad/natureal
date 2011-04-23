#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include "settings.h"
#include "Texture.h"

class TextureManager
{
public:
	TextureManager(void);
	~TextureManager(void);

	int loadTexture(string filename,string inShaderName = "not_in_shader_texture");
	void bindTexture(int texId, GLenum texUnit);
	void unbindTexture(int texId);

private:
	vector<Texture*> textures;
};

#endif

