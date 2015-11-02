#pragma once

#include <GL/glew.h>
#include <iostream>

#include "Mesh.h"

using namespace std;
class Cylinder : public Mesh
{
private:
	int nverts;
public:
	void display();
	Cylinder(string id);
	~Cylinder();
};

