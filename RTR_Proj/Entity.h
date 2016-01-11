#pragma once

#include "AttacheableObject.h"

class Mesh;

using namespace std;

class Entity;

struct SubEntity
{
	Mesh *mesh;
	int material;
	int materialToUse;
	float distanceToCam;
	int model;
	Entity *entity;

	bool isInFrustum(Camera *camera, bool omniCam = false);
	float distanceToCamera(Camera *camera) const;
	bool operator<(const SubEntity& se) const;
	static bool less(SubEntity *se1, SubEntity *se2);
};

/*class sorter {
	Camera *cam;
public:
	sorter(Camera *cam) : cam(cam) {}
	bool operator()(SubEntity const* se1, SubEntity const* se2) const;
};*/

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
	void update(float seconds) override;
	RenderOrder getRenderEntities(int material, Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;
};

