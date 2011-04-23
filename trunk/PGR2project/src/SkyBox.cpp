#include "SkyBox.h"
SkyBox::SkyBox():
	SceneModel(NULL)
{
}

SkyBox::SkyBox(TextureManager *texManager, string filename):
	SceneModel(texManager)
{
	// load textures...
	char fname[100];
	char shname[100];
	int id;
	for (int i=0; i<6; i++)
	{
		sprintf(fname, filename.c_str(),appendices[i]); 
		sprintf(shname, "skybox_tex_%s", appendices[i]);
		id = textureManager->loadTexture(string(fname), string(shname));
		textureIds.push_back(id);
	}
}


SkyBox::~SkyBox(void)
{
}

void SkyBox::draw()
{

}

void SkyBox::init()
{

}

void SkyBox::update(float time)
{

}

void SkyBox::translate(v3 &movVector)
{

}

void SkyBox::rotate(v3 &axis, float angleRad)
{

}

void SkyBox::scale(v3 &scaleVector)
{

}

void SkyBox::loadFromFiles(string filename)
{

}
