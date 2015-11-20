#include "SceneManager.h"

SceneManager::SceneManager()
{
	this->root = std::make_unique<SceneNode>("_root", this);
	this->materials = map<const string, Material*>();
}



void SceneManager::addMaterial(char *id, char *diffuse, char *specular, char *vert, char *frag, Material::shaderTypes shaderType)
{
	auto s = new Shader(vert, frag);
	auto m = new Material(id, diffuse, specular, s, shaderType, this);
	this->materials.insert(pair<const string, Material*>(id, m));
}



void SceneManager::addMaterial(char* id, char *diffuse, char *specular, Shader* shader, Material::shaderTypes shaderType)
{
	auto m = new Material(id, diffuse, specular, shader, shaderType, this);
	this->materials.insert(pair<const string, Material*>(id, m));
}



void SceneManager::setDefaultMaterial(char *id)
{
	this->defaultMaterial = id;
}



const char* SceneManager::getDefaultMaterialId()
{
	return defaultMaterial;
}


Material* SceneManager::getMaterial(const char *material)
{
	return this->materials.find(material)->second;
}



shared_ptr<Entity> SceneManager::createEntity(char *id, char *mesh)
{
	return nullptr;
}



shared_ptr<Entity> SceneManager::createEntity(char *id, Mesh *mesh)
{
	return std::make_shared<Entity>(id, mesh, this);
}



shared_ptr<Light> SceneManager::createLight(char* id, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, char* mesh)
{
	return nullptr;
}



shared_ptr<Light> SceneManager::createLight(char* id, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, Mesh* mesh)
{
	auto l = std::make_shared<Light>(id, ambient, diffuse, specular, constant, linear, quadratic, mesh, this);
	lights.push_back(l);
	return l;
}



void SceneManager::createDirectionalLight(char* id, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* direction)
{
	delete directionalLight;
	this->directionalLight = new Light("directional", ambient, diffuse, specular, direction, this);
}



vector<shared_ptr<Light>> SceneManager::getActiveLights()
{
	auto a_l = vector<shared_ptr<Light>>();
	for (auto i = 0; i < lights.size(); i++)
	{
		if(lights[i]->getParent())
		{
			a_l.push_back(lights[i]);
		}
	}

	return a_l;
}

Light* SceneManager::getDirectionalLight()
{
	return directionalLight;
}


SceneNode* SceneManager::getRoot()
{
	return root.get();
}


void SceneManager::Update(float seconds)
{
	root->Update(seconds);
}

void SceneManager::render(Camera *camera)
{
	auto defMaterial = this->getMaterial(defaultMaterial);

	if (!defMaterial)
		return;

	auto mat = glm::mat4();
	root->display(mat, defaultMaterial, camera);
}