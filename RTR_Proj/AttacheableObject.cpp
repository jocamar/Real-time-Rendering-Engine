#include "AttacheableObject.h"
#include "SceneManager.h"

AttacheableObject::AttacheableObject(const char *id, SceneManager *manager, SceneNode *parent)
{
	this->id = id;
	this->material = -1;
	this->manager = manager;
	this->parent = parent;
}



AttacheableObject::AttacheableObject(const char *id, SceneManager *manager, char *material, SceneNode *parent)
{
	this->id = id;
	this->material = manager->getMaterialNum(material);
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
	this->material = manager->getMaterialNum(material);
}



int AttacheableObject::getMaterial()
{
	return material;
}



SceneManager* AttacheableObject::getManager()
{
	return manager;
}