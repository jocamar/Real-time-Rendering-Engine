#pragma once
#include "SceneNode.h"

#include <map>
#include "Material.h"
#include "Entity.h"
#include "Light.h"


class SceneManager
{
protected:
	unique_ptr<SceneNode> root;
	map<const string, Material*> materials;
	vector<shared_ptr<Light>> lights;
	Light *directionalLight;
	char* defaultMaterial;

public:
	SceneManager();
	void addMaterial(char *id, char *diffuse, char *specular, char *vert, char *frag, Material::shaderTypes shaderType = Material::LIGHTING);
	void addMaterial(char *id, char *diffuse, char *specular, Shader *shader, Material::shaderTypes shaderType = Material::LIGHTING);
	void setDefaultMaterial(char* id);
	const char* getDefaultMaterialId();
	Material* getMaterial(const char* material);

	shared_ptr<Entity> createEntity(char *id, char *mesh);
	shared_ptr<Entity> createEntity(char *id, Mesh *mesh);

	shared_ptr<Light> createLight(char *id, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, char *mesh);
	shared_ptr<Light> createLight(char *id, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, Mesh *mesh = nullptr);

	void createDirectionalLight(char *id, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat *direction);

	vector<shared_ptr<Light>> getActiveLights();
	Light* getDirectionalLight();

	SceneNode* getRoot();

	void render(Camera *camera);
};