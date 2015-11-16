#include "Entity.h"
#include "SceneManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

Entity::Entity(string idEntity, Mesh *MeshEntity, SceneManager *manager) : AttacheableObject(manager) 
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
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(s->Program, "model");
	GLint viewLoc = glGetUniformLocation(s->Program, "view");
	GLint projLoc = glGetUniformLocation(s->Program, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transf));
	EntityMesh->display();
}


bool Entity::isLeaf()
{
	return true;
}
