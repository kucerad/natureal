#include "Light.h"


Light::Light()
{

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
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glTranslate(position);
		glColor(v3(1,1,1));
		drawCube();
	glPopMatrix();
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
