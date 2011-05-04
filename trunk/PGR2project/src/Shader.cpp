#include "Shader.h"

Shader::Shader(void)
{
	programID	 = 0;
	vertexShID	 = 0;
	geometryShID = 0;
	fragmentShID = 0;
}

Shader::~Shader(void)
{
	printf("Shader deleting...\n");
	destroy();
	printf("Shader destroyed.\n");
}

bool Shader::loadShader(string vs_filename, string fs_filename)
{
	const char* pVertexShaderFileName    = vs_filename.c_str();
	const char* pFragmentShaderFileName	 = fs_filename.c_str();
	printf("# COMPILE SHADERS\n");

	destroy();

	GLint bCompiled			 = false;
	bool  bAddVertexShader	 = (pVertexShaderFileName != NULL);
	bool  bAddFragmentShader = (pFragmentShaderFileName != NULL);

	if (bAddVertexShader)
	{
 		// Create the vertex shader_________________________________________________
		vertexShID = glCreateShader(GL_VERTEX_SHADER);
			
		const char* pVSsource = readFile(pVertexShaderFileName);
		glShaderSource (vertexShID, 1, (const char**) &pVSsource, NULL );
		glCompileShader(vertexShID);
		delete [] pVSsource;

		glGetShaderiv(vertexShID, GL_COMPILE_STATUS, &bCompiled);
		printShaderInfoLog(vertexShID, false);
		if (bCompiled  == false)
		{
			printf(" - Vertex shader compilation failed...\n\n");
			return false;
		}
		printf(" + Vertex shader compiled...\n");
	}

	// Create the fragment shader_______________________________________________
	if (bAddFragmentShader)
	{
	    fragmentShID = glCreateShader(GL_FRAGMENT_SHADER);
			
		const char* pFSsource = readFile(pFragmentShaderFileName);
		glShaderSource (fragmentShID, 1, (const char**) &pFSsource, NULL );
		glCompileShader(fragmentShID);
		delete [] pFSsource;

		glGetShaderiv(fragmentShID, GL_COMPILE_STATUS, &bCompiled);
		printShaderInfoLog(fragmentShID, false);
		if (bCompiled  == false)
		{
			printf(" - Fragment shader compilation failed...\n\n");
			return false;
		}
		printf(" + Fragment shader compiled...\n");
	}

    // Create a program object and attach the two compiled shaders______________
	GLint bLinked	= false;

	programID = glCreateProgram();
	if (bAddVertexShader) 
	{
		glAttachShader(programID, vertexShID  );
	}
	if (bAddFragmentShader) 
	{
		glAttachShader(programID, fragmentShID);
	}

	// Link the program object and print out the info log_______________________
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &bLinked);
	printShaderInfoLog(programID, true);
	if (bLinked == false)
	{
		printf("- Shader program linking failed...\n\n");
		return false;
	}
	timeLocation = glGetUniformLocation(programID, "time");
	printf(" (time location: %i)\n", timeLocation);
	printf("+ SUCCESS\n");

	return true;
}

bool Shader::loadShader(
		string vs_filename,
		string fs_filename,
		string gs_filename,
		GLint  geometry_vertices_out,
		GLenum geometry_primitive_in,
		GLenum geometry_primitive_out
		)
{
	const char* pVertexShaderFileName    = vs_filename.c_str();
	const char* pGeometryShaderFileName  = gs_filename.c_str();
	const char* pFragmentShaderFileName	 = fs_filename.c_str();
						
	printf("# COMPILE SHADERS\n");

	destroy();

	GLint bCompiled			 = false;
	bool  bAddVertexShader	 = (pVertexShaderFileName != NULL);
	bool  bAddGeometryShader = (pGeometryShaderFileName != NULL);
	bool  bAddFragmentShader = (pFragmentShaderFileName != NULL);

	if (bAddVertexShader)
	{
 		// Create the vertex shader_________________________________________________
		vertexShID = glCreateShader(GL_VERTEX_SHADER);
			
		const char* pVSsource = readFile(pVertexShaderFileName);
		glShaderSource (vertexShID, 1, (const char**) &pVSsource, NULL );
		glCompileShader(vertexShID);
		delete [] pVSsource;

		glGetShaderiv(vertexShID, GL_COMPILE_STATUS, &bCompiled);
		printShaderInfoLog(vertexShID, false);
		if (bCompiled  == false)
		{
			printf(" - Vertex shader compilation failed...\n\n");
			return false;
		}
		printf(" + Vertex shader compiled...\n");
	}

	if (bAddGeometryShader)
	{
 		// Create the vertex shader_________________________________________________
		geometryShID = glCreateShader(GL_GEOMETRY_SHADER_EXT);
			
		const char* pGSsource = readFile(pGeometryShaderFileName);
		glShaderSource (geometryShID, 1, (const char**) &pGSsource, NULL );
		glCompileShader(geometryShID);
		delete [] pGSsource;

		glGetShaderiv(geometryShID, GL_COMPILE_STATUS, &bCompiled);
		printShaderInfoLog(geometryShID, false);
		if (bCompiled  == false)
		{
			printf(" - Geometry shader compilation failed...\n\n");
			return false;
		}
		printf(" + Geometry shader compiled...\n");
	}

	// Create the fragment shader_______________________________________________
	if (bAddFragmentShader)
	{
	    fragmentShID = glCreateShader(GL_FRAGMENT_SHADER);
			
		const char* pFSsource = readFile(pFragmentShaderFileName);
		glShaderSource (fragmentShID, 1, (const char**) &pFSsource, NULL );
		glCompileShader(fragmentShID);
		delete [] pFSsource;

		glGetShaderiv(fragmentShID, GL_COMPILE_STATUS, &bCompiled);
		printShaderInfoLog(fragmentShID, false);
		if (bCompiled  == false)
		{
			printf(" - Fragment shader compilation failed...\n\n");
			return false;
		}
		printf(" + Fragment shader compiled...\n");
	}

    // Create a program object and attach the two compiled shaders______________
	GLint bLinked	= false;

	programID = glCreateProgram();
	
	if (bAddVertexShader) 
	{
		glAttachShader(programID, vertexShID  );
	}
	if (bAddGeometryShader) 
	{
		glAttachShader(programID, geometryShID  );
	}
	if (bAddFragmentShader) 
	{
		glAttachShader(programID, fragmentShID);
	}

	// Set shader program parameters for geometry shader
	if (bAddGeometryShader) 
	{
		glProgramParameteriEXT(programID, GL_GEOMETRY_VERTICES_OUT_EXT, geometry_vertices_out);
		glProgramParameteriEXT(programID,   GL_GEOMETRY_INPUT_TYPE_EXT, geometry_primitive_in);
		glProgramParameteriEXT(programID,  GL_GEOMETRY_OUTPUT_TYPE_EXT, geometry_primitive_out);
	}

	// Link the program object and print out the info log_______________________
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &bLinked);
	printShaderInfoLog(programID, true);
	if (bLinked == false)
	{
		printf(" - Shader program linking failed...\n\n");
		return false;
	}
	printf("+ SUCCESS.\n");
	return true;


}

void Shader::use(bool isTurnedOn)
{
	if (isTurnedOn){
		glUseProgram(programID);
		// attach uniforms
		int i, size = uniforms.size();
		for (i=0; i<size; i++){
			glUniform1i(uniforms[i].location, uniforms[i].value);
		}
	} else {
		glUseProgram(0);
	}
}

void Shader::setTime(float in_time)
{
	//printf("set time: %f, loc: %i\n", in_time, timeLocation);
	glUniform1f(timeLocation, in_time);
}

bool Shader::linkTexture(Texture *texture)
{
	GLint loc = glGetUniformLocation(programID, texture->inShaderName.c_str());
	if (loc < 0){
		return false;
	}
	Uniformi uniform;
	uniform.location = loc;
	uniform.value	 = texture->textureUnitNumber;
	uniforms.push_back(uniform);
}

void Shader::printShaderInfoLog(GLuint objectId, bool bShaderProgram)
{
	int		infoLogLength = 0;
	int		writtenChars  = 0;
	GLchar* pBuffer		  = NULL;

	if (bShaderProgram)
	{
		glGetProgramiv(objectId, GL_INFO_LOG_LENGTH, &infoLogLength);
	}
	else
	{
		glGetShaderiv(objectId, GL_INFO_LOG_LENGTH, &infoLogLength);
	}

//	assert(glGetError() == GL_NO_ERROR);

	if (infoLogLength > 0)
	{
		pBuffer = new GLchar[infoLogLength];

		if (bShaderProgram)
		{
			glGetProgramInfoLog(objectId, infoLogLength, &writtenChars, pBuffer);
		}
		else
		{
			glGetShaderInfoLog(objectId, infoLogLength, &writtenChars, pBuffer);
		}

		printf("%s\n-----------------------------------------\n", pBuffer);

		delete [] pBuffer;
	}
}

void Shader::destroy()
{
	if (programID != 0)			
	{
		if (vertexShID != 0)
		{
			glDeleteShader(vertexShID);
			vertexShID	= 0;
		}
		if (geometryShID != 0)
		{
			glDeleteShader(geometryShID);
			geometryShID	= 0;
		}
		if (fragmentShID != 0)
		{
			glDeleteShader(fragmentShID);
			fragmentShID	= 0;
		}

		glDeleteProgram(programID);
		programID = 0;
	}
}

int		Shader::getLocation(string var_name)
{
	int loc = glGetUniformLocation(programID, var_name.c_str());
	if (loc>=0){
		// valid uniform
		int out = locations.size();
		locations.push_back(loc);
		return out;
	}
	return -1;
}
void	Shader::setUniform1i(int locIndex, int value)
{
	glUniform1i(locations[locIndex], value);
}
void    Shader::setUniform2i(int locIndex, int value1, int value2)
{
	glUniform2i(locations[locIndex], value1, value2);
}
void	Shader::setUniform3i(int locIndex, int value1, int value2, int value3)
{
	glUniform3i(locations[locIndex], value1, value2, value3);
}
void    Shader::setUniform4i(int locIndex, int value1, int value2, int value3, int value4)
{
	glUniform4i(locations[locIndex], value1, value2, value3, value4);
}
void	Shader::setUniform1f(int locIndex, float value)
{
	glUniform1f(locations[locIndex], value);
}
void    Shader::setUniform2f(int locIndex, float value1, float value2)
{
	glUniform2f(locations[locIndex], value1, value2);
}
void	Shader::setUniform3f(int locIndex, float value1, float value2, float value3)
{
	glUniform3f(locations[locIndex], value1, value2, value3);
}
void    Shader::setUniform4f(int locIndex, float value1, float value2, float value3, float value4)
{
	glUniform4f(locations[locIndex], value1, value2, value3, value4);
}
void	Shader::setUniformfp(int locIndex, float* values, int count)
{
	glUniform4fv(locations[locIndex], count, values);
}
void    Shader::setUniform2v(int locIndex, v2 value)
{
	glUniform2f(locations[locIndex],value.x, value.y);
}
void	Shader::setUniform3v(int locIndex, v3 value)
{
	glUniform3f(locations[locIndex], value.x, value.y, value.z);
}
void    Shader::setUniform4v(int locIndex, v4 value)
{
	glUniform4f(locations[locIndex], value.x, value.y, value.z, value.w);
}