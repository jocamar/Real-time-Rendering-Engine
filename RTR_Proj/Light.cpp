#include "Light.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Mesh.h"

Light::Light(const char *idLight, SceneManager* manager, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, const char *modelId, SceneNode* parent) : Entity(idLight, manager, modelId, parent)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	this->directional = false;
	this->direction = nullptr;
}



Light::Light(const char *idLight, SceneManager* manager, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* direction) : Entity(idLight, manager, nullptr)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->directional = true;
	this->direction = direction;
}



void Light::display(glm::mat4 transf, int material, Camera* camera)
{
	for(auto se : subEntities)
	{
		se.mesh->display(transf, material, camera);
	}
}



bool Light::isLeaf()
{
	return true;
}



bool Light::isDirectional()
{
	return directional;
}



GLfloat* Light::getAmbient()
{
	return ambient;
}



GLfloat* Light::getDiffuse()
{
	return diffuse;
}



GLfloat* Light::getSpecular()
{
	return specular;
}



GLfloat* Light::getDirection()
{
	return direction;
}



GLfloat Light::getConstant()
{
	return constant;
}



GLfloat Light::getLinear()
{
	return linear;
}



GLfloat Light::getQuadratic()
{
	return quadratic;
}