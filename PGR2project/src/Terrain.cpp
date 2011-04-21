#include "Terrain.h"


Terrain::Terrain(void)
{
	heightMap		= NULL;
	dim_x			= 0;
	dim_y			= 0;
	drawingMethod	= GL_TRIANGLE_STRIP;
	vboId			= 0;
	vboCount		= 0;
	eboId			= 0;
	eboCount		= 0;

	shaderID		= 0;
}


Terrain::~Terrain(void)
{
	delete [] heightMap;
	heightMap = NULL;
}

void Terrain::draw()
{
	glUseProgram(shaderID);

		// set data_texture
		//glUniform1i(locations[BRANCH_DATA_TEXTURE], heightTexture->unitId);
			
		// bind index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,eboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId); 
		   // enable states
		   glEnableClientState(GL_VERTEX_ARRAY);
		   glEnableClientState(GL_NORMAL_ARRAY);
		   glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		   // draw VBOs...
			glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
			glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET( vboCount*sizeof(GLfloat) ) );
			//glClientActiveTexture(dataTexture->unitId);
			//glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET());
			
			glDrawElements(GL_TRIANGLE_STRIP, eboCount, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
			
		   // disable
		   
		   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		   glDisableClientState(GL_NORMAL_ARRAY);
		   glDisableClientState(GL_VERTEX_ARRAY);
		// unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// turn off shader
	glUseProgram(0);
	// unbind data texture
	//dataTexture->unbindTexture();


}
	 
void Terrain::init()
{
	// load heightmap
	loadHeightMap(HEIGHTMAP_SOURCE); 

	drawingMethod = GL_TRIANGLE_STRIP;
	// create grid of triangles
	glGenBuffers(1, &eboId);
	glGenBuffers(1, &vboId);
	int ch = 3;
	float step_x = 1.f, step_y = 1.f;
	GLfloat * vertices = new GLfloat[dim_x*dim_y*ch];
	GLfloat * normals  = new GLfloat[dim_x*dim_y*ch];
	GLuint  * elements = new GLuint[(dim_x-1)*2*dim_y];
	int x,y;
	vboCount = 0;
	float hCorrection = getHeightAt(dim_x/2, dim_y/2);

	for (x=0; x<dim_x; x++){
		for (y=0; y<dim_y; y++){
			vertices[(x*dim_y + y)*ch + 0] = x*step_x - dim_x/2.0;//x
			vertices[(x*dim_y + y)*ch + 1] = getHeightAt(x,y);//height
			vertices[(x*dim_y + y)*ch + 2] = y*step_y - dim_y/2.0;//y
			v3 normal;
			normal.x = getHeightAt(x-1,y) - getHeightAt(x+1,y);
			normal.y = getHeightAt(x,y);
			normal.z = getHeightAt(x,y+1)-getHeightAt(x,y+1);
			normal.normalize();
			normals[(x*dim_y + y)*ch + 0] = normal.x;
			normals[(x*dim_y + y)*ch + 1] = normal.y;
			normals[(x*dim_y + y)*ch + 2] = normal.z;
			vboCount += 3;
		}
	}
	int eli = 0;
	bool reverse = false;
	for (x=0; x<dim_x-1; x++){
		if (!reverse){
			for (y=0; y<dim_y; y++){
				elements[eli] = (x+1)*dim_y + y;
				eli++;
				elements[eli] = (x)*dim_y + y;
				eli++;
			}
		} else {
			for (y=dim_y-1; y>=0; y--){
				elements[eli] = (x)*dim_y + y;
				eli++;
				elements[eli] = (x+1)*dim_y + y;
				eli++;
			}
		}
		reverse = !reverse;
	}
	eboCount = eli;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboCount*sizeof(GLuint), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
		// alloc space
		glBufferData(GL_ARRAY_BUFFER, 2*vboCount*sizeof(GLfloat), 0, GL_STATIC_DRAW); 
		// fill vertices
		glBufferSubData(GL_ARRAY_BUFFER,0, vboCount*sizeof(GLfloat), vertices); 
		// fill normals
		glBufferSubData(GL_ARRAY_BUFFER, vboCount*sizeof(GLfloat), 2*vboCount*sizeof(GLfloat), normals);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
	
void Terrain::update(float time)
{
	
}
	 
void Terrain::translate(v3 movVector)
{

}
	 
void Terrain::rotate(v3 axis, float angleRad)
{
	
}
	 
void Terrain::scale(v3 scaleVector)
{

}
	 
void Terrain::loadHeightMap(string filename)
{
	PNG inputFile;
	inputFile.load(filename);
	dim_x = inputFile.width;
	dim_y = inputFile.height;
	unsigned char * data = inputFile.getData();

	int size = dim_x*dim_y;
	heightMap = new float[dim_x*dim_y];

	for (int i=0; i<size; i++){
		heightMap[i] = 256*data[2*i]+data[2*i + 1];
	}
}


float Terrain::getHeightAt(float x, float y)
{
	return 0.0;
}
float Terrain::getHeightAt(int x, int y)
{
	x = min(max(x,0),dim_x-1);
	y = min(max(y,0),dim_y-1);
	float hCorrection = heightMap[dim_x/2*dim_y + dim_y/2];
	return HEIGHTMAP_SCALE*(heightMap[x*dim_y + y]-hCorrection)+HEIGHTMAP_INITHEIGHT;	
}
