#pragma once

#include <iostream>
#include "Mesh.h"

using namespace std;
class Cube : public Mesh
{
public:
	Cube(const char *id, SceneManager *manager);
	void display(glm::mat4 transf, int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;
	~Cube();
};

