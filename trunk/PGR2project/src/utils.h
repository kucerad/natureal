#ifndef _UTILS_H
#define _UTILS_H

#include "settings.h"
#define BACKSPACE(cnt) for (int i=0; i<cnt; i++){printf("\b \b");}
#define BACK(cnt) for (int i=0; i<cnt; i++){printf("\b");}

#define SAFE_DELETE_PTR(VAR) if(VAR!=NULL){delete VAR; VAR=NULL;}
#define SAFE_DELETE_ARRAY_PTR(VAR) if(VAR!=NULL){delete [] VAR; VAR=NULL;}


static void show_texture(GLuint texId, GLint x,GLint y, GLsizei width, GLsizei height){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, g_WinWidth, 0, g_WinHeight, -1,1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texId);
	glColor4f(1.f,1.f,1.f,1.f);

	glBegin(GL_QUADS);
		glTexCoord2f(0.f, 0.f); glVertex2i(x,y);
		glTexCoord2f(1.f, 0.f); glVertex2i(x+width,y);
		glTexCoord2f(1.f, 1.f); glVertex2i(x+width,y+height);
		glTexCoord2f(0.f, 1.f); glVertex2i(x,y+height);
	glEnd();

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

static void glScale(v3 &scaleFactors)
{
	glScalef(scaleFactors.x, scaleFactors.y, scaleFactors.z);
};
static void glTranslate(v3 &movement)
{
	glTranslatef(movement.x, movement.y, movement.z);
};
static void glRotate(v3 &axis, float angle)
{
	glRotatef(angle, axis.x, axis.y, axis.z);
};
static void glColor(v3 &color)
{
	glColor3f(color.x, color.y, color.z);
};

#endif


