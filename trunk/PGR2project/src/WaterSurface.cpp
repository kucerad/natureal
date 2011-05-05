#include "WaterSurface.h"


WaterSurface::WaterSurface(TextureManager *texManager, ShaderManager *shManager):
	SceneModel(texManager, shManager)
{

}

WaterSurface::~WaterSurface(void)
{
	// destroy buffers
	glDeleteFramebuffersEXT(1, &fbID);
	glDeleteTextures(1, &cbID);
	glDeleteTextures(1, &dbID);
}

void WaterSurface::draw()
{
	shader->use(true);
	shader->setUniform1i(water_reflection_loc, 0);// texture unit 0
	glColor3f(0.f, 0.1f, 1.f);
	// use texture
	glPushMatrix();
	glScalef(500.f, 1.f, 500.f);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		
		glBindTexture(GL_TEXTURE_2D, cbID);
			glClientActiveTexture(GL_TEXTURE0);
			drawPlane();
		glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glPopMatrix();
	shader->use(false);

}


void WaterSurface::init()
{
	// init textures & buffers
	//textureManager->loadTexture(...);

	glGenTextures(1, &cbID);
		glBindTexture(GL_TEXTURE_2D, cbID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_WinWidth, g_WinHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
   
	   
	glGenTextures(1, &dbID );
		glBindTexture(GL_TEXTURE_2D, dbID );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, g_WinWidth, g_WinHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	 glGenFramebuffersEXT(1, &fbID);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbID);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, cbID, 0);
		//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, color_tex1_id, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, dbID, 0);
	   

		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	 assert(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)==GL_FRAMEBUFFER_COMPLETE_EXT);
	 assert( glGetError() == GL_NO_ERROR );
	 printf("WATER framebuffers initialized successfully, fb %i,cb %i, db %i\n", fbID, cbID, dbID);
	 
	 // init shaders...
	 int shaderID = shaderManager->loadShader(WATER_VS_FILENAME, WATER_FS_FILENAME);
	 shader = shaderManager->getShader(shaderID);
 	 water_reflection_loc = shader->getLocation("water_reflection");
}

void WaterSurface::update(double time)
{

}

void WaterSurface::translate(v3 &movVector)
{

}

void WaterSurface::rotate(v3 &axis, float angleRad)
{

}

void WaterSurface::scale(v3 &scaleVector)
{

}

void WaterSurface::windowSizeChanged(int width, int height)
{
	// reinit framebuffers...
		glBindTexture(GL_TEXTURE_2D, cbID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, dbID );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, 0 );
}

void WaterSurface::beginReflection()
{
	glEnable(GL_CULL_FACE);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbID);
    // prenastavit viewport
		glViewport(0,0,g_WinWidth, g_WinHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//glDrawBuffersARB(2, buffers);
		// nastavit transformaci...
		glPushMatrix();
		
		
		if (activeCamera->getPosition().y>=WATER_HEIGHT){
			glTranslatef(0.0f, WATER_HEIGHT, 0.0f);
			glScalef(1.f, -1.f, 1.f); // flip Y
			glCullFace(GL_FRONT);
		} else {
			glScalef(1.f, 0.5f, 1.f);
			glTranslatef(0.0f, WATER_HEIGHT-1.f, 0.0f);
		}
		
		//double plane[4] = {0.0, 1.0, 0.0, -0.5};  // slow on my GPU
		//glEnable(GL_CLIP_PLANE0);
		//glClipPlane(GL_CLIP_PLANE0, plane);//GLenum buffers[2] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT};//, GL_DEPTH_ATTACHMENT_EXT };
}


void WaterSurface::endReflection()
{
	// vymazat transformaci
	glPopMatrix();
	// viewport zpatky...
		//glDisable(GL_CLIP_PLANE0); // slow on my GPU
	if (activeCamera->getPosition().y>=WATER_HEIGHT){
		glCullFace(GL_BACK);
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glViewport(0,0,g_WinWidth, g_WinHeight);
	
}

void WaterSurface::showTexture(int x, int y, int szx, int szy)
{
	show_texture(cbID, x,y,szx,szy);
}