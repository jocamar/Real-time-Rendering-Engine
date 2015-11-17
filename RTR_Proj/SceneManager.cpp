#include "SceneManager.h"

SceneManager::SceneManager()
{
	this->root = std::make_unique<SceneNode>("_root", this);
	this->shaders = map<const string, Shader*>();
}



void SceneManager::addShader(char *id, char *vert, char *frag)
{
	this->shaders.insert(pair<const string, Shader*>(id, new Shader(vert, frag)));
}



void SceneManager::setDefaultShader(char *id)
{
	this->defaultShader = id;
}



const char* SceneManager::getDefaultShaderId()
{
	return defaultShader;
}


Shader* SceneManager::getShader(const char *shader)
{
	return this->shaders.find(shader)->second;
}



shared_ptr<Entity> SceneManager::createEntity(char *id, char *mesh)
{
	return nullptr;
}



shared_ptr<Entity> SceneManager::createEntity(char *id, Mesh *mesh)
{
	return std::make_shared<Entity>(id, mesh, this);
}



SceneNode* SceneManager::getRoot()
{
	return root.get();
}

void SceneManager::render(Camera *camera)
{
	auto defShader = this->getShader(defaultShader);

	if (!defShader)
		return;

	auto mat = glm::mat4();
	root->display(mat, defaultShader, camera);
}