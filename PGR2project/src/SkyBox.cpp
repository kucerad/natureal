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
		sprintf(fname, filename.c_str(),appendices[i].c_str()); 
		sprintf(shname, "skybox_tex_%s", appendices[i].c_str());
		id = textureManager->loadTexture(string(fname), string(shname));
		textureIds.push_back(id);
	}
}


SkyBox::~SkyBox(void)
{
}

void SkyBox::draw()
{
	glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	glMatrixMode(GL_MODELVIEW);
	glTranslate(p_activeCamera->getPosition());
	glScale(v3(SKYBOX_SIZE));
	
	// down 
	glPushMatrix();
	glTranslatef(0,-0.499,0);
	glRotatef(180, 1,0,0);
	textureManager->bindTexture(textureIds[5], GL_TEXTURE0);
	drawPlane();
	textureManager->unbindTexture(textureIds[5]);
	glPopMatrix();
	
	// up 
	glPushMatrix();	
	glTranslatef(0,0.499,0);	
	textureManager->bindTexture(textureIds[4], GL_TEXTURE0);
	drawPlane();
	textureManager->unbindTexture(textureIds[4]);
	glPopMatrix();
	

	// north
	glPushMatrix();
	glTranslatef(0, 0, -0.499);
	glRotatef(-90, 1,0,0);
	glScale(v3(-1));
	textureManager->bindTexture(textureIds[0], GL_TEXTURE0);
	drawPlane();
	textureManager->unbindTexture(textureIds[0]);
	glPopMatrix();
	
	// south
	glPushMatrix();
	glTranslatef(0, 0, 0.499);
	glRotatef(90, 1,0,0);
	textureManager->bindTexture(textureIds[3], GL_TEXTURE0);
	drawPlane();
	textureManager->unbindTexture(textureIds[3]);
	glPopMatrix();
	
	// east 
	glPushMatrix();
	glTranslatef(-0.499, 0,0);
	glRotatef(90, 0,0,1);
	textureManager->bindTexture(textureIds[2], GL_TEXTURE0);
	drawPlane();
	textureManager->unbindTexture(textureIds[2]);
	glPopMatrix();
	
	// west 
	glPushMatrix();
	glTranslatef(0.499,0,0);
	glRotatef(-90, 0,0,1);
	textureManager->bindTexture(textureIds[1], GL_TEXTURE0);
	drawPlane();	
	textureManager->unbindTexture(textureIds[1]);
	glPopMatrix();
	
	glEnable(GL_LIGHTING);
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
