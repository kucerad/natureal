#include "ShaderManager.h"


ShaderManager::ShaderManager(void)
{
}


ShaderManager::~ShaderManager(void)
{
	int size = shaders.size();
	for (int i=0; i<size; i++){
		SAFE_DELETE_PTR(shaders[i]);
	}
}

int ShaderManager::loadShader(string vs_filename, string fs_filename)
{
	int out = shaders.size();
	Shader* sh = new Shader();
	sh->loadShader(vs_filename, fs_filename);
	shaders.push_back(sh);
	return out;
}

int ShaderManager::loadShader(	string vs_filename,
				string fs_filename,
				string gs_filename,
				GLint  geometry_vertices_out,
				GLenum geometry_primitive_in,
				GLenum geometry_primitive_out)
{
	int out = shaders.size();
	Shader* sh = new Shader();
	sh->loadShader(vs_filename, fs_filename, gs_filename, geometry_vertices_out, geometry_primitive_in, geometry_primitive_out);
	shaders.push_back(sh);
	return out;
}

void ShaderManager::use(int shaderId, bool turnOn)
{
	if (shaderId<shaders.size() && shaderId>=0){
		shaders[shaderId]->use(turnOn);
	}
}