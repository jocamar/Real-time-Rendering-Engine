#pragma once

#include <iostream>
#include "Mesh.h"

using namespace std;
class Cube : public Mesh
{
public:
	Cube(const char *id, SceneManager *manager);
	void display(glm::mat4 transf, int material, Camera *camera = nullptr, bool shadowMap = false) override;
	~Cube();
};

