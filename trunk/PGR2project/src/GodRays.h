#ifndef _GODRAYS_H
#define _GODRAYS_H

#include "ShaderManager.h"
#include "settings.h"
#include "utils.h"
#include "Light.h"

class GodRays
{
public:
	GodRays(ShaderManager *shManager, Light *_light);
	~GodRays(void);

	void begin();
	void end();

	void windowSizeChanged(int width, int height);

	GLfloat			 lightDirDOTviewDirValue;
private:
	GLuint			 fboId;
	GLuint			 originalColor;
	GLint			 originalColorLocation;
	GLuint			 originalDepth;	
	GLuint			 forRaysColor;
	GLint			 forRaysColorLocation;
	GLint			 lightDOTviewLocation;

	GLint			 lightPosLocation;
	ShaderManager	*shaderManager;
	Shader			*shader;
	Light			*light;

};

#endif

