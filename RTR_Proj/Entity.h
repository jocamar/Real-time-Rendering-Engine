#pragma once

#include "AttacheableObject.h"

class Mesh;

using namespace std;
class Entity : public AttacheableObject
{
private:
	string idEntity;
	Mesh* EntityMesh;
public:
	Entity(string idEntity, Mesh *EntityMesh, SceneManager *manager, SceneNode *parent = nullptr);
	void display(glm::mat4 transf, char *material, Camera *camera = nullptr) override;
	bool isLeaf() override;
};

