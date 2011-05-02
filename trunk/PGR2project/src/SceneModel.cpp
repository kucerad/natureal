#include "SceneModel.h"

SceneModel::SceneModel(){
	textureManager = NULL;
}
SceneModel::SceneModel(TextureManager *texManager, ShaderManager *shManager){
	textureManager = texManager;
	shaderManager  = shManager;
}


SceneModel::~SceneModel(){

}