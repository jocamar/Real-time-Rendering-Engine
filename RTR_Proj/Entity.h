#pragma once

#include <GL/glew.h>
#include <iostream>

#include "Mesh.h"

using namespace std;
class Entity
{
private:
	string idEntity;
	Mesh* EntityMesh;
public:
	Entity(string idEntity, Mesh* EntityMesh);
	void display();
};

