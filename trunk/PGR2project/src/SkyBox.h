#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "scenemodel.h"
class SkyBox :
	public SceneModel
{
public:
	SkyBox(void);
	~SkyBox(void);

	void draw();

	void init();

	void update(float time);

	void translate(v3 movVector);

	void rotate(v3 axis, float angleRad);

	void scale(v3 scaleVector);

	void loadFromFiles(string filename);
};

#endif

