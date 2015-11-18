#include "Entity.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Mesh.h"

Entity::Entity(string idEntity, Mesh *EntityMesh, SceneManager *manager, SceneNode *parent) : AttacheableObject(manager, parent) 
{
	this->idEntity = idEntity;
	this->EntityMesh = EntityMesh;
}



void Entity::display(glm::mat4 transf, char *material, Camera *camera) {
	
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


bool Entity::isLeaf()
{
	return true;
}
