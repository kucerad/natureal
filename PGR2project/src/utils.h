#ifndef _UTILS_H
#define _UTILS_H

#include "settings.h"

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


