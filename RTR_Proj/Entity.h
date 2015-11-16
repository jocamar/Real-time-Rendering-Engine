#pragma once

#include <iostream>

#include "Mesh.h"
#include "AttacheableObject.h"

using namespace std;
class Entity : public AttacheableObject
{
private:
	string idEntity;
	Mesh* EntityMesh;
public:
	Entity(string idEntity, Mesh *EntityMesh, SceneManager *manager);
	void display(glm::mat4 transf, char *shader, Camera *camera = nullptr) override;
	bool isLeaf() override;
};

