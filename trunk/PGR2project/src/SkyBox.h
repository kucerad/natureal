#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "SceneModel.h"
#include "Camera.h"

static const string appendices[] = {
	"north",
	"west",
	"east",
	"south",
	"up",
	"down"
};

class SkyBox :
	public SceneModel
{
public:
	SkyBox(void);
	SkyBox(TextureManager *texManager, ShaderManager *shManager, string filename);
	~SkyBox(void);

	void draw();

	void init();

	void update(float time);

	void translate(v3 &movVector);

	void rotate(v3 &axis, float angleRad);

	void scale(v3 &scaleVector);

	void loadFromFiles(string filename);

	vector<int> textureIds;
	Camera* p_activeCamera;
};

#endif

