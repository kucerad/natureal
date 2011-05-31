#ifndef _SHADER_H
#define _SHADER_H

#include "settings.h"
#include "../common/Vector2.h"
#include "../common/Vector3.h"
#include "../common/Vector4.h"
#include "ioutils.h"
#include "Texture.h"
#include "utils.h"

struct Uniformi{
	GLint location;
	GLint value;
};

class Shader
{
public:
	Shader(string name);
	~Shader(void);

	GLuint programID;
	bool	loadShader(	string vs_filename, string fs_filename);
	bool	loadShader(
						string vs_filename,
						string fs_filename,
						string gs_filename,
						GLint  geometry_vertices_out,
						GLenum geometry_primitive_in,
						GLenum geometry_primitive_out
					   );
	bool	linkTexture(Texture *texture);
	void	setTime(float in_time);
	void	destroy();
	void	use(bool isTurnedOn);
	int		getLocation(string var_name);
	void	setUniform1i(int locIndex, int value);
	void    setUniform2i(int locIndex, int value1, int value2);
	void	setUniform3i(int locIndex, int value1, int value2, int value3);
	void    setUniform4i(int locIndex, int value1, int value2, int value3, int value4);
	void	setUniform1f(int locIndex, float value);
	void    setUniform2f(int locIndex, float value1, float value2);
	void	setUniform3f(int locIndex, float value1, float value2, float value3);
	void    setUniform4f(int locIndex, float value1, float value2, float value3, float value4);
	void	setUniformfp(int locIndex, float* values, int count);
	void    setUniform2v(int locIndex, v2 value);
	void	setUniform3v(int locIndex, v3 value);
	void    setUniform4v(int locIndex, v4 value);
	void	setUniformMatrix(int locIndex, m4 & matrix);
	void	setBoolean(int locIndex, bool value);

	GLint	timeLocation;
	vector<GLint>	 locations;
private:	

	void   printShaderInfoLog(GLuint objectId, bool bShaderProgram);

	GLuint vertexShID;
	GLuint geometryShID;
	GLuint fragmentShID;

	vector<Uniformi> uniforms;
	string name;
	

};

#endif
