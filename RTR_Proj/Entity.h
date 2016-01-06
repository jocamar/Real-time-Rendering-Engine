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

	bool isInFrustum(Camera *camera);
	float distanceToCamera(Camera *camera);
};

class Entity : public AttacheableObject
{
protected:
	int model;
	vector<SubEntity> subEntities;
	bool shadowCaster;
public:
	Entity(const char *idEntity, SceneManager *manager, const char *modelId, SceneNode *parent = nullptr, bool shadowCaster = true);
	void display(int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;
	bool isLeaf() override;
	void update(float seconds) override {};
	RenderOrder getRenderEntities(int material, Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;
};

