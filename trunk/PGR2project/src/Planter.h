#ifndef _PLANTER_H
#define _PLANTER_H

#include "settings.h"
#include "Terrain.h"
#include "../common/Vector2.h"
#include "Vegetation.h"
#include <ctime>

class Planter
{
public:
	Planter(Terrain *_terrain);
	~Planter(void);
	
	int plantVegetation(Vegetation* prototype, Vegetation* growth);



	Terrain *terrain;	
};

#endif

