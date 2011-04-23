#include "Terrain.h"


Terrain::Terrain(TextureManager *texManager):
	SceneModel(texManager)
{
	heightMap		= NULL;
	dim_x			= 0;
	dim_y			= 0;
	drawingMethod	= GL_TRIANGLE_STRIP;
	vboId			= 0;
	vboCount		= 0;
	eboId			= 0;
	eboCount		= 0;
	vertices 		= NULL;
	normals  		= NULL;
	elements 		= NULL;
	shaderID		= 0;
	reduction		= 1;
}


Terrain::~Terrain(void)
{
	SAFE_DELETE_ARRAY_PTR( heightMap );
	SAFE_DELETE_ARRAY_PTR(vertices);
	SAFE_DELETE_ARRAY_PTR(normals);
	SAFE_DELETE_ARRAY_PTR(elements);
}

void Terrain::draw()
{
	glUseProgram(shaderID);
		
		// bind textures
		for (int i=0; i<TERRAIN_TEX_COUNT; i++){
			textureManager->bindTexture(textureIds[i], GL_TEXTURE0+GLuint(i));
		}
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
			glVertexPointer(channels[VERTEX], glTypes[VERTEX], 0, BUFFER_OFFSET(offsets[VERTEX]));
			glNormalPointer(glTypes[NORMAL], 0, BUFFER_OFFSET( offsets[NORMAL] ) );
			glClientActiveTexture(GL_TEXTURE0);
			glTexCoordPointer(channels[TEXCOORD0], glTypes[TEXCOORD0], 0, BUFFER_OFFSET(offsets[TEXCOORD0]));
			
			glDrawElements(GL_TRIANGLE_STRIP, eboCount, GL_UNSIGNED_INT, BUFFER_OFFSET(offsets[INDEX]));
			
		   // disable
		   
		   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		   glDisableClientState(GL_NORMAL_ARRAY);
		   glDisableClientState(GL_VERTEX_ARRAY);
		// unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// unbind textures
		for (int i=0; i<TERRAIN_TEX_COUNT; i++){
			textureManager->unbindTexture(textureIds[i]);
		}
	// turn off shader
	glUseProgram(0);
}

void Terrain::drawNormals()
{
	int numVertices = vboCount/3;
	glDisable(GL_LIGHTING);
	glColor(v3(1.f,0.f,0.f));
	glBegin(GL_LINES);
	float x,y,z;
	for (int i=0; i<numVertices; i++){
		x = vertices[i*3 + 0];
		y = vertices[i*3 + 1];
		z = vertices[i*3 + 2];
		glVertex3f(x,y,z);
		x += normals[i*3 + 0];
		y += normals[i*3 + 1];
		z += normals[i*3 + 2];
		glVertex3f(x,y,z);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}
	 
void Terrain::init()
{
	// load heightmap
	loadHeightMap(HEIGHTMAP_SOURCE);

	// load textures
	loadTextures(TERRAIN_TEX_NAME, TERRAIN_TEX_COUNT);

	reduction = 10;
	dim_x = hdim_x/reduction;
	dim_y = hdim_y/reduction;
	drawingMethod = GL_TRIANGLE_STRIP;
	// create grid of triangles
	glGenBuffers(1, &eboId);
	glGenBuffers(1, &vboId);
	int ch = 3;
	float step_x = 1.f, step_y = 1.f;
	step_x *=reduction;
	step_y *=reduction;
	vertices = new GLfloat[dim_x*dim_y*ch];
	normals  = new GLfloat[dim_x*dim_y*ch];
	texCoords  = new GLfloat[dim_x*dim_y*2];
	elements = new GLuint[(dim_x-1)*2*dim_y];

	channels	[INDEX]		= 1;
	channels	[VERTEX]	= 3;
	channels	[NORMAL]	= 3;
	channels	[TEXCOORD0] = 2;

	typeSizes	[INDEX]		= sizeof(GLuint);
	typeSizes	[VERTEX]	= sizeof(GLfloat);
	typeSizes	[NORMAL]	= sizeof(GLfloat);
	typeSizes	[TEXCOORD0] = sizeof(GLfloat);

	glTypes		[INDEX]		= GL_UNSIGNED_INT;
	glTypes		[VERTEX]	= GL_FLOAT;
	glTypes		[NORMAL]	= GL_FLOAT;
	glTypes		[TEXCOORD0] = GL_FLOAT;

	for (int k = 0; k < VBO_ATR_COUNT; k++){
			sizes[k]=0;
	}
	sizes		[INDEX]		= (dim_x-1)*2*dim_y * channels[INDEX]	* typeSizes[INDEX];
	sizes		[VERTEX]	= dim_x * dim_y * channels[VERTEX]	* typeSizes[VERTEX];
	sizes		[NORMAL]	= dim_x * dim_y * channels[NORMAL]	* typeSizes[NORMAL];
	sizes		[TEXCOORD0] = dim_x * dim_y * channels[TEXCOORD0]* typeSizes[TEXCOORD0];

	offsets		[INDEX]		= 0;
	offsets		[VERTEX]	= 0;
	offsets		[NORMAL]	= offsets [VERTEX] + sizes [VERTEX];
	offsets		[TEXCOORD0] = offsets [NORMAL] + sizes [NORMAL];
	
	int x,y,n;
	vboCount = 0;
	float tex_cnt_x = 100.f;
	float tex_cnt_y = 100.f;

	sz_x = dim_x*reduction;
	sz_y = dim_y*reduction;
	float sx2 = sz_x/2;
	float sy2 = sz_y/2;
	int hx, hy;
	for (x=0; x<dim_x; x++){
		for (y=0; y<dim_y; y++){
			hx = x*reduction;
			hy = y*reduction;
			vertices[(x*dim_y + y)*ch + 0] = x*step_x - sx2;//x
			vertices[(x*dim_y + y)*ch + 1] = getHeightAt(hx,hy);//height
			vertices[(x*dim_y + y)*ch + 2] = y*step_y - sy2;//y
			
			// normals
			v3 normal;
			
			normal.x = getHeightAt(hx-reduction,hy) - getHeightAt(hx+reduction,hy);
			normal.y = reduction;
			normal.z = getHeightAt(hx,hy-reduction) - getHeightAt(hx,hy+reduction);
			normal.normalize();

			if (normal.y<0){
				normal = -normal;
			}
			
			normals[(x*dim_y + y)*ch + 0] = normal.x;
			normals[(x*dim_y + y)*ch + 1] = normal.y;
			normals[(x*dim_y + y)*ch + 2] = normal.z;

			// texCoords
			texCoords[(x*dim_y + y)*2 + 0] = x * tex_cnt_x / dim_x;
			texCoords[(x*dim_y + y)*2 + 1] = y * tex_cnt_y / dim_y;

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

	// total vbo buffer size
	int vboSize=0;
	for (int b = 0; b < VBO_ATR_COUNT; b++){
		if (b!=INDEX){
			vboSize+=sizes[b];
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizes[INDEX], elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
		// alloc space
		glBufferData(GL_ARRAY_BUFFER, vboSize, 0, GL_STATIC_DRAW); 
		// fill vertices
		glBufferSubData(GL_ARRAY_BUFFER, offsets[VERTEX], sizes[VERTEX], vertices); 
		// fill normals
		glBufferSubData(GL_ARRAY_BUFFER, offsets[NORMAL], sizes[NORMAL], normals);
		// fill texcoords
		glBufferSubData(GL_ARRAY_BUFFER, offsets[TEXCOORD0], sizes[TEXCOORD0], texCoords);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
	
void Terrain::update(float time)
{
	
}
	 
void Terrain::translate(v3 &movVector)
{

}
	 
void Terrain::rotate(v3 &axis, float angleRad)
{
	
}
	 
void Terrain::scale(v3 &scaleVector)
{

}
	 
void Terrain::loadHeightMap(string filename)
{
	PNG inputFile;
	inputFile.load(filename);
	hdim_x = inputFile.width;
	hdim_y = inputFile.height;
	unsigned char * data = inputFile.getData();

	int size = hdim_x*hdim_y;
	heightMap = new float[size];

	for (int i=0; i<size; i++){
		heightMap[i] = 256*data[2*i]+data[2*i + 1];
	}
}

void Terrain::loadTextures(string filename, int count){
	char def_filename [100];
	char sh_name [100];
	int texId;
	for (int i=0; i<count; i++){
		sprintf(def_filename, filename.c_str(), i+1);
		sprintf(sh_name, "terrain_tex_%02i", i+1);
		texId = textureManager->loadTexture( string(def_filename) , string(sh_name));
		textureIds.push_back(texId);
	}

}

float Terrain::getHeightAt(float x, float y)
{
	// interpolate height
	int ix,iy;
	ix = (int) x;
	iy = (int) y;
	return getHeightAt(ix,iy);
	/*
	float tx = x-ix;
	float ty = y-iy;
	float h3 = (1-ty)*getHeightAt(ix,iy) + ty*getHeightAt(ix, iy+1);
	float h4 = (1-ty)*getHeightAt(ix+1,iy) + ty*getHeightAt(ix+1, iy+1); 
	return (1-tx)*h3 + (tx)*h4;
	*/
}
float Terrain::getHeightAt(int x, int y)
{
	int dx = hdim_x;
	int dy = hdim_y;
	x = min(max(x,0),dx-1);
	y = min(max(y,0),dy-1);
	float hCorrection = heightMap[dx/2*dy + dy/2];
	//printf("h[%i, %i, dy=%i, s=%i] = %f\n", x,y,dy,x*dy + y, HEIGHTMAP_SCALE*(heightMap[x*dy + y]-hCorrection)+HEIGHTMAP_INITHEIGHT);
	return HEIGHTMAP_SCALE*(heightMap[x*dy + y]-hCorrection)+HEIGHTMAP_INITHEIGHT;	
}
