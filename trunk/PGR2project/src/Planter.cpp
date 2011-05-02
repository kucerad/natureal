#include "Planter.h"

Planter::Planter(Terrain *_terrain):
	terrain(_terrain)
{

}

Planter::~Planter(void)
{

}
	
int Planter::plantVegetation(Vegetation* prototype, Vegetation *growth)
{
	// fill all available place with prototype copy...
	float height_min = 0.0f;
	float height_max = 10.0f;
	int desiredCount = 2;
	int count = 0;
	float xmin = -10.0;
	float xmax =  10.0;
	float ymin = -10.0;
	float ymax =  10.0;
	float tsx = terrain->sz_x/2.0;
	float tsy = terrain->sz_y/2.0;
	float minDist = 0.0f;
	v3 distVector;
	int i;
	bool valid;

	// copy non-vertex data
	//growth = prototype->getCopy();


	// get candidate positions 
	while (count<desiredCount){
		// generate random position
		float x = randomf(xmin, xmax);
		float z = randomf(ymin, ymax);
		float y = terrain->getHeightAt(x+tsx,z+tsy);
		v3 rpos = v3(x,y,z);
		// validate
		valid = true;
		/*
		for (i=0; i<plants.size(); i++)
		{
			distVector = plants[i]->position - rpos;
			if (distVector.length() < minDist){
				// not OK? -> generate new position (continue) 
				valid = false;
				break;
			}
		}*/
		// is OK? -> save, count++;
		if (valid){
			m4 transform;
			transform.rotate(v3(0.f, 1.f, 0.f), randomf(0.f, PI/4.f));
			// transform normal
			//m4 normalTransform(transform);

			transform.scale(v3(randomf(0.5f, 1.5f)));
			transform.translate(rpos);
			// transform all vertices of vegetation
			for (int i=0; i<prototype->VBOdataCount; i++)
			{
				VertexInfo v(prototype->vertices[i]);
				v.position = transform*v.position;
				v.normal   = (transform*v.normal);
				v.normal.normalize();
				growth->vertices.push_back(v);
			}			
			count++;
		}
	}
	return count;
}
