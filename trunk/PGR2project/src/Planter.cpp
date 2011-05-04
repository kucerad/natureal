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
	//int desiredCount = 5000;
	int count = 0;
	float xmin = -100.0;
	float xmax =  100.0;
	float ymin = -100.0;
	float ymax =  100.0;
	float tsx = terrain->sz_x/2.0;
	float tsy = terrain->sz_y/2.0;
	float minDist = 1.5f;
	v3 distVector;
	int i;
	bool valid;

	// get candidates array
	int res_x = 100;
	int res_y = 100;
	float step_x = terrain->sz_x/float(res_x);
	float step_y = terrain->sz_y/float(res_y);
	vector<v3> candidates;
	int xi, yi;
	for (xi=0; xi<res_x; xi++)
	{
		for (yi=0; yi<res_y; yi++)
		{
			float x = randomf(-step_x*0.5f, step_x*0.5f);
			float y = randomf(-step_y*0.5f, step_y*0.5f);
			v3  pos(xi*step_x+x, 0.f , yi*step_y+y);
			float height = terrain->getHeightAt(xi*step_x+x, yi*step_y+y);
			if (height>height_min && height<height_max){
				pos.y=height;
				pos.x -= tsx;
				pos.z -= tsy;
				candidates.push_back(pos); 
			}
		}
	}
	// get positions from candidates
	while (!candidates.empty()){
		// select candidate
		v3 candidate = candidates[candidates.size()-1];
		
		// delete all neighbour candidates closer than minDist
		for (int i=candidates.size()-1; i>=0; i-- ){
			v3 distVector = candidate - candidates[i];
			float dist = distVector.length();
			if (dist<minDist){
				// closer -> delete
				candidates.erase(candidates.begin()+i);
			}
		}
		// save position
		m4 transform;
		transform.scale(v3(randomf(0.3f, 1.1f)));
		transform.rotate(v3(0.f, 1.f, 0.f), randomf(0.f, PI/4.f));
		transform.translate(candidate);
		prototype->fixTexType();
		// transform all vertices of vegetation
		for (int i=0; i<prototype->VBOdataCount; i++)
		{
			VertexInfo v(prototype->vertices[i]);
			//printf("pos: %f, %f, %f, %f\n", v.position.x, v.position.y, v.position.z, v.position.w);
			v.position = transform*v.position;
			//printf("popos: %f, %f, %f, %f\n", v.position.x, v.position.y, v.position.z, v.position.w);
			v.texCoord = prototype->transformTexCoords(v.texCoord);	
			v.normal   = (transform*v.normal);
			v.normal.normalize();
			growth->vertices.push_back(v);
		}			
		count++;
		int k = printf("VEG count: %i", count);
		BACKSPACE(k);

	} // end while

	printf("VEG count: %i\n", count);
	/*

	// get candidate positions 
	while (count<desiredCount){
		// generate random position
		float x = randomf(xmin, xmax);
		float z = randomf(ymin, ymax);
		float y = terrain->getHeightAt(x+tsx,z+tsy);
		v3 rpos = v3(x,y,z);
		// validate
		valid = true;
		
		for (i=0; i<plants.size(); i++)
		{
			distVector = plants[i]->position - rpos;
			if (distVector.length() < minDist){
				// not OK? -> generate new position (continue) 
				valid = false;
				break;
			}
		}
		// is OK? -> save, count++;
		if (valid){
			m4 transform;
			transform.scale(v3(randomf(0.3f, 1.1f)));
			transform.rotate(v3(0.f, 1.f, 0.f), randomf(0.f, PI/4.f));
			// transform normal
			//m4 normalTransform(transform);
			//printf("rpos: %f, %f, %f\n", rpos.x, rpos.y, rpos.z);
			transform.translate(rpos);
			//transform.printOut();
			// transform all vertices of vegetation
			for (int i=0; i<prototype->VBOdataCount; i++)
			{
				VertexInfo v(prototype->vertices[i]);
				//printf("pos: %f, %f, %f, %f\n", v.position.x, v.position.y, v.position.z, v.position.w);
				v.position = transform*v.position;
				//printf("popos: %f, %f, %f, %f\n", v.position.x, v.position.y, v.position.z, v.position.w);
				
				v.normal   = (transform*v.normal);
				v.normal.normalize();
				growth->vertices.push_back(v);
			}			
			count++;
		}
		*/
	return count;
}
