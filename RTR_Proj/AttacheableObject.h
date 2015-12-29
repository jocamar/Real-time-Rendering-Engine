#pragma once
#include "Globals.h"

class Camera;
class SceneManager;
class SceneNode;

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

	virtual void display(glm::mat4 transf, int material, Camera *camera = nullptr) = 0;
	virtual bool isLeaf() = 0;
	virtual void update(float seconds) = 0;

	SceneNode* getParent();
	void setParent(SceneNode *parent);
	void setMaterial(char *material);

	virtual ~AttacheableObject() {};
};
