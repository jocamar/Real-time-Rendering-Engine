#pragma once

#include <GL/glew.h>
#include <iostream>

#include "Mesh.h"

using namespace std;
class Triangle : public Mesh
{
private:

public:
	void display();
	Triangle(string id);
	~Triangle();
};

