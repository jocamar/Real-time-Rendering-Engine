#pragma once

#include <GL/glew.h>
#include <iostream>

#include "Mesh.h"

using namespace std;
class Cube : public Mesh
{
private:

public:
	void display();
	Cube(string id);
	~Cube();
};

