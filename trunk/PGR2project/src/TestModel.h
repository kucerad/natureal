#pragma once
#include "scenemodel.h"
#include "..\common\models\elephant.h"

class TestModel :
	public SceneModel
{
public:
	TestModel(void);
	~TestModel(void);

	virtual void draw();

	virtual void drawForLOD();

	virtual void init();

	virtual void update(double time);

	Shader *shader;
};

