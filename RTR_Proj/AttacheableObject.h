#pragma once
#include "Globals.h"

class Camera;
class SceneManager;
class SceneNode;
struct SubEntity;
struct RenderOrder;

class AttacheableObject
{
protected:
	const char *id;
	SceneManager *manager;
	SceneNode *parent;
	int material;
public:
	explicit AttacheableObject(const char *id, SceneManager *manager, SceneNode *parent = nullptr);
	AttacheableObject(const char *id, SceneManager *manager, char *material, SceneNode *parent = nullptr);

	virtual void display(int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) = 0;
	virtual bool isLeaf() = 0;
	virtual void update(float seconds) = 0;
	virtual RenderOrder getRenderEntities(int material, Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) = 0;

	SceneNode* getParent();
	void setParent(SceneNode *parent);
	void setMaterial(char *material);
	int getMaterial();
	SceneManager* getManager();

	virtual ~AttacheableObject() {};
};
