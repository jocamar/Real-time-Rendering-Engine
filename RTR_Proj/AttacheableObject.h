#pragma once
#include "Globals.h"

class Camera;
class SceneManager;
class SceneNode;

class AttacheableObject
{
protected:
	SceneManager *manager;
	SceneNode *parent;
	char *shader;
public:
	explicit AttacheableObject(SceneManager *manager, SceneNode *parent = nullptr);
	AttacheableObject(SceneManager *manager, char *shader, SceneNode *parent = nullptr);
	virtual void display(glm::mat4 transf, char *shader, Camera *camera = nullptr) = 0;
	virtual bool isLeaf() = 0;
	virtual ~AttacheableObject() {};
	SceneNode* getParent();
	void setParent(SceneNode *parent);
};
