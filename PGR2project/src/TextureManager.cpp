#include "TextureManager.h"


TextureManager::TextureManager(void)
{
	shadowMapID = -1;
}


TextureManager::~TextureManager(void)
{
	for (int i=0; i<textures.size(); i++){
		SAFE_DELETE_PTR(textures[i]);
	}
}

void TextureManager::activateTexture(int texId)
{
	textures[texId]->activate();
}

void TextureManager::deactivateTexture(int texId)
{
	textures[texId]->deactivate();
}

int TextureManager::addTexture(Texture * texture)
{
	textures.push_back(texture);
	return textures.size()-1;
}

Texture * TextureManager::getTexture(int texId)
{
	return textures[texId];
}


int TextureManager::loadTexture(string filename,string inShaderName, GLint unitNumber, bool buildMipmaps, GLint wrapMode, GLint filterMode)
{
	Texture *texture = new Texture();
	texture->load(filename, unitNumber, buildMipmaps, wrapMode, filterMode);
	texture->inShaderName = inShaderName;
	textures.push_back(texture);
	return textures.size()-1;
}
void TextureManager::bindTexture(int texId, GLenum texUnit)
{
	if (texId>=0 && texId<textures.size()){
		textures[texId]->bind(texUnit);
	}
}
void TextureManager::unbindTexture(int texId)
{
	if (texId>=0 && texId<textures.size()){
		textures[texId]->unbind();
	}
}
