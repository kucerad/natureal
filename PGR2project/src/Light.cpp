#include "Light.h"


Light::Light(TextureManager * tm)
{
	positionFixedToSkybox = v3(0.0,0.0,0.0);
	textureManager = tm;
}


Light::~Light(void)
{

}
void Light::init()
{
	glGenTextures(1, &db_shad_ID );
		glBindTexture(GL_TEXTURE_2D, db_shad_ID );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWMAP_RESOLUTION_X, SHADOWMAP_RESOLUTION_Y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glGenTextures(1, &cb_shad_ID );
		glBindTexture(GL_TEXTURE_2D, cb_shad_ID );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SHADOWMAP_RESOLUTION_X, SHADOWMAP_RESOLUTION_Y, 0, GL_RGBA, GL_FLOAT, 0);
	 glGenFramebuffersEXT(1, &fb_shad_ID);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb_shad_ID);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, db_shad_ID, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, cb_shad_ID, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	Texture * shadowMapTexture = new Texture();
	shadowMapTexture->id = db_shad_ID;
	shadowMapTexture->inShaderName = "shadowMap";
	shadowMapTexture->textureUnitNumber = 7; // last texture
	shadowMapTMID = textureManager->addTexture(shadowMapTexture);

}

void Light::update(double time)
{

}

void Light::translate(v3 &movVector)
{
	position += movVector;
}

void Light::rotate(v3 &axis, float angleRad)
{
	direction.rotate(angleRad, axis);
}

void Light::scale(v3 &scaleVector)
{

}

void Light::showTextures(){
	show_texture(cb_shad_ID, 0,200,200,200);
	show_texture(db_shad_ID, 200,200,200,200);
}
void Light::beginShadowMap(){
	// set camera to light position and set its params...
	float fov = 100.f;
	float near = 100.f;
	float far = 1000.f;
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb_shad_ID);
    // prenastavit viewport
		glViewport(0,0,SHADOWMAP_RESOLUTION_X, SHADOWMAP_RESOLUTION_Y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective(fov, (GLfloat)SHADOWMAP_RESOLUTION_X/(GLfloat)SHADOWMAP_RESOLUTION_Y, near, far);  
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		v3 p(LIGHT_POSITION);
		gluLookAt(p.x				, p.y				, p.z				,
					p.x-LIGHT_POSITION.x	, p.y-LIGHT_POSITION.y	, p.z -LIGHT_POSITION.z	,
					0.0		, 1.0		, 0.0		);

	// redirect renderign to framebuffer
	
	// set viewport
	//glViewport(0, 0, SHADOWMAP_RESOLUTION_X, SHADOWMAP_RESOLUTION_Y);
	// ... render scene at fastest settings
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

}
void Light::endShadowMap(){
	// restore rendering settings
	glPopAttrib();

	// redirect rendering back to back screen buffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// reset viewport
	glViewport(0, 0, g_WinWidth, g_WinHeight);
	// set camera...
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	// SHOW SHADOW MAP
	showTextures();
}

void Light::draw()
{
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslate(g_light_position);
		glColor4f(1.0,1.0,0.6,1.0);
		glutSolidSphere(10.0, 5, 5);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glPopAttrib();
}

void Light::turnOn()
{
	glEnable(GL_LIGHTING);
	glEnable(lightId);
}

void Light::turnOff()
{
	glDisable(lightId);
}

void Light::setup(GLuint lid, v3 &pos, v3 &dir, v4 &ambi, v4 &diff, v4 &spec, float cutOff, float exp)
{
	lightId = lid;
	position = pos;
	direction= dir;
	glLightfv(lid, GL_POSITION      , v4(pos).data);
	glLightfv(lid, GL_SPOT_DIRECTION, v4(dir).data);
	glLightfv(lid, GL_AMBIENT       , ambi.data);
	glLightfv(lid, GL_DIFFUSE       , diff.data);
	glLightfv(lid, GL_SPECULAR	    , spec.data);
	glLightf (lid, GL_SPOT_CUTOFF	, cutOff);
	glLightf (lid, GL_SPOT_EXPONENT , exp);
}
