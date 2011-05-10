#include "Light.h"


Light::Light()
{
	positionFixedToSkybox = v3(0.0,0.0,0.0);
}


Light::~Light(void)
{

}
void Light::init()
{
	
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
