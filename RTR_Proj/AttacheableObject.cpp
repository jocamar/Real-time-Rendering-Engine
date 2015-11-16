#include "AttacheableObject.h"

AttacheableObject::AttacheableObject(SceneManager *manager, SceneNode *parent)
{
	this->shader = nullptr;
	this->manager = manager;
	this->parent = parent;
}



AttacheableObject::AttacheableObject(SceneManager *manager, char *shader, SceneNode *parent)
{
	this->shader = shader;
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