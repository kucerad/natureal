#ifndef _VEGETATION_H
#define _VEGETATION_H


#include "scenemodel.h"
#include "terrain.h"


struct VertexInfo
{
	v4 position;
	v4 normal;
	v3 texCoord;
	static const int sizePosition	= 3;
	static const int sizeNormal		= 3;
	static const int sizeTexCoord	= 2;
};

struct Offsets
{
	int position;
	int normal;
	int texCoord;
};


class Vegetation :
	public SceneModel
{
public:
	Vegetation(TextureManager *texManager, ShaderManager *shManager);
	virtual ~Vegetation(void);
	
	virtual Vegetation* getCopy()=0;

	virtual void draw()=0;

	virtual void init()=0;

	virtual void update(double time)=0;

	virtual void translate(v3 &movVector)=0;

	virtual void rotate(v3 &axis, float angleRad)=0;

	virtual void scale(v3 &scaleVector)=0;

	virtual void bakeToVBO()=0;

	virtual void fixTexType()=0;
	virtual v3	 transformTexCoords(v3 &origTexCoords)=0;

	vector<VertexInfo>	vertices;
	v3					position;
	v3					scaleVector;
	float				rotationY;

	float				*pVBOdata;
	Offsets				offsets;
	int					VBOdataCount;
	int					VBOdataSize;
	int					*pEBOdata;
	int					EBOdataCount;
};

#endif
