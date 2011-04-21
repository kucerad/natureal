#ifndef __SCENEMODEL__
#define __SCENEMODEL__


#include "settings.h"

class SceneModel
{
public:
	SceneModel(void);
	~SceneModel(void);

	virtual void draw()=0;

	virtual void init()=0;

	virtual void update(float time)=0;

	virtual void translate(v3 movVector)=0;

	virtual void rotate(v3 axis, float angleRad)=0;

	virtual void scale(v3 scaleVector)=0;
};
#endif

