#include "Light.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Mesh.h"

Light::Light(string idEntity, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, Mesh* EntityMesh, SceneManager* manager, SceneNode* parent) : AttacheableObject(manager, parent)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	this->idEntity = idEntity;
	this->EntityMesh = EntityMesh;
	this->directional = false;
	this->direction = nullptr;
}

Light::Light(string idEntity, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* direction, SceneManager* manager) : AttacheableObject(manager)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->directional = true;
	this->direction = direction;
	this->idEntity = idEntity;
	this->EntityMesh = nullptr;
}

void Light::display(glm::mat4 transf, char* material, Camera* camera)
{
	char* materialToUse;
	if (this->material)
		materialToUse = this->material;
	else
		materialToUse = material;

	auto s = this->manager->getMaterial(material);

	s->use(camera);

	// Create camera transformation
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)800 / (float)600, 0.1f, 1000.0f);

	// Pass the matrices to the shader
	glUniformMatrix4fv(s->getShader()->ViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(s->getShader()->ProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(s->getShader()->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));
	EntityMesh->display();
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