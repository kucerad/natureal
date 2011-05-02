#ifndef _SHADER_H
#define _SHADER_H

#include "settings.h"
#include "ioutils.h"
#include "Texture.h"

struct Uniformi{
	GLint location;
	GLint value;
};

class Shader
{
public:
	Shader(void);
	~Shader(void);

	GLuint programID;
	bool loadShader(string vs_filename, string fs_filename);
	bool loadShader(
		string vs_filename,
		string fs_filename,
		string gs_filename,
		GLint  geometry_vertices_out,
		GLenum geometry_primitive_in,
		GLenum geometry_primitive_out
		);
	bool linkTexture(Texture *texture);
	void setTime(float in_time);
	void destroy();
	void use(bool isTurnedOn);

	GLint  timeLocation;

private:	

	void   printShaderInfoLog(GLuint objectId, bool bShaderProgram);

	GLuint vertexShID;
	GLuint geometryShID;
	GLuint fragmentShID;
	

	vector<Uniformi> uniforms;
	

};

#endif
