#pragma once

#include "AttacheableObject.h"

class Mesh;

using namespace std;

class Entity;

struct SubEntity
{
	Mesh *mesh;
	int material;
	int model;
	Entity *entity;
};

class Entity : public AttacheableObject
{
protected:
	int model;
	vector<SubEntity> subEntities;
	bool shadowCaster;
public:
	Entity(const char *idEntity, SceneManager *manager, const char *modelId, SceneNode *parent = nullptr, bool shadowCaster = true);
	void display(glm::mat4 transf, int material, Camera *camera = nullptr, bool shadowMap = false) override;
	bool isLeaf() override;
	void update(float seconds) override {};
};

