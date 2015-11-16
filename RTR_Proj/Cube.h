#pragma once

#include <GL/glew.h>
#include <iostream>

#include "Mesh.h"
#include <glm/mat4x4.hpp>

using namespace std;
class Cube : public Mesh
{
private:

public:
	void display() override;
	Cube(string id);
	~Cube();
};

