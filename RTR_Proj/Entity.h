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
public:
	Entity(const char *idEntity, SceneManager *manager, const char *modelId, SceneNode *parent = nullptr);
	void display(glm::mat4 transf, int material, Camera *camera = nullptr) override;
	bool isLeaf() override;
	void update(float seconds) override {};
};

