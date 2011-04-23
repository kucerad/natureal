#ifndef _LIGHT_H
#define _LIGHT_H


#include "scenemodel.h"
class Light :
	public SceneModel
{
public:
	Light(void);
	~Light(void);
	void draw();

	void init();

	void update(float time);

	void translate(v3 &movVector);

	void rotate(v3 &axis, float angleRad);

	void scale(v3 &scaleVector);

	void turnOn();
	void turnOff();
	void setup(GLuint lid, v3 &pos, v3 &dir, v4 &ambi, v4 &diff, v4 &spec, float cutOff, float exp);

// attributes

	v3 position;
	v3 direction;
	GLuint lightId;
};

#endif

