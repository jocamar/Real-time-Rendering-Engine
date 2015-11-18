#include "AttacheableObject.h"

AttacheableObject::AttacheableObject(SceneManager *manager, SceneNode *parent)
{
	this->material = nullptr;
	this->manager = manager;
	this->parent = parent;
}



AttacheableObject::AttacheableObject(SceneManager *manager, char *material, SceneNode *parent)
{
	this->material = material;
	this->manager = manager;
	this->parent = parent;
}

SceneNode* AttacheableObject::getParent()
{
	return parent;
}


void AttacheableObject::setParent(SceneNode* parent)
{
	this->parent = parent;
}



void AttacheableObject::setMaterial(char* material)
{
	this->material = material;
}