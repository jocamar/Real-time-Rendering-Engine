#include "Entity.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Mesh.h"

Entity::Entity(string idEntity, Mesh *MeshEntity, SceneManager *manager, SceneNode *parent) : AttacheableObject(manager, parent) 
{
	this->idEntity = idEntity;
	this->EntityMesh = MeshEntity;
}



void Entity::display(glm::mat4 transf, char *shader, Camera *camera) {
	
	char* shaderToUse;
	if (this->shader)
		shaderToUse = this->shader;
	else
		shaderToUse = shader;

	auto s = this->manager->getShader(shader);

	s->Use();

	// Create camera transformation
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)800 / (float)600, 0.1f, 1000.0f);

	// Pass the matrices to the shader
	glUniformMatrix4fv(s->ViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(s->ProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(s->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));
	EntityMesh->display();
}


bool Entity::isLeaf()
{
	return true;
}
