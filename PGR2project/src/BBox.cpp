#include "BBox.h"


BBox::BBox(void)
{
}
BBox::BBox(v3 &_minCorner, v3 &_maxCorner, v3 &_color)
{
	center  	= (_minCorner + _maxCorner)/2.0;
	sizes		= _maxCorner - _minCorner;
	color		= _color;
	drawMode    = GL_LINE;
	minCorner   = _minCorner;
}

BBox::~BBox(void)
{
}

	
void BBox::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glColor(color);
	//glPushAttrib( GL_POLYGON_BIT );
	
	glPushMatrix();
	glTranslate(minCorner);
	glScale(sizes);
	
	glPolygonMode(GL_FRONT_AND_BACK, drawMode);
	drawCube();
	glPopMatrix();
	//glPopAttrib();

}