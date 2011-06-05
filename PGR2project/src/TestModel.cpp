#include "TestModel.h"


TestModel::TestModel(void)
{
}


TestModel::~TestModel(void)
{
}

void TestModel::draw()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat material_amd[4] = {1.0f, 0.1f, 0.1f, 1.0f};
	GLfloat material_spe[4] = {0.8f, 0.8f, 0.8f, 1.0f};
	
	glMaterialfv(GL_FRONT_AND_BACK,  GL_AMBIENT, material_amd);
	glMaterialfv(GL_FRONT_AND_BACK,  GL_DIFFUSE, material_amd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_spe);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

	shader->use(true);
	glPushMatrix();
		glColor3f(1.0, 0.5, 0.5);
		glTranslatef(0.0f, 8.f, 0.f);
		glScalef(5.f, 5.f, 5.f);
		pgr2DrawElephant();
	glPopMatrix();
	shader->use(false);
}

void TestModel::drawForLOD()
{

}

void TestModel::init()
{

}

void TestModel::update(double time)
{

}
