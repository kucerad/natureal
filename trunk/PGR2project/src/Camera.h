#ifndef _CAMERA_H
#define _CAMERA_H

#include "settings.h"


class Camera
{
public:
	Camera(void);
	~Camera(void);

	/*void move(v3 &movement);
	void moveOnTerrain(v3 &movement);
	void lookAt(v3 &lookPoint);*/


	// attributes
	v3 upVector;
	v3 dirVector;
	v3 position;

};

#endif
