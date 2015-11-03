#include "Entity.h"

Entity::Entity(string idEntity, Mesh* MeshEntity) {
	this->idEntity = idEntity;
	this->EntityMesh = MeshEntity;
}

void Entity::display() {
	EntityMesh->display();
}