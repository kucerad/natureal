#include "SceneModel.h"

SceneModel::SceneModel(){
	textureManager = NULL;
}
SceneModel::SceneModel(TextureManager *texManager){
	textureManager = texManager;
}


SceneModel::~SceneModel(){

}