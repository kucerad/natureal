#ifndef _UTILS_H
#define _UTILS_H

#include "settings.h"
#define BACKSPACE(cnt) for (int i=0; i<cnt; i++){printf("\b \b");}
#define BACK(cnt) for (int i=0; i<cnt; i++){printf("\b");}

#define SAFE_DELETE_PTR(VAR) if(VAR!=NULL){delete VAR; VAR=NULL;}
#define SAFE_DELETE_ARRAY_PTR(VAR) if(VAR!=NULL){delete [] VAR; VAR=NULL;}


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


