#pragma once

#include <GL/glew.h>
#include <iostream>

#include "Mesh.h"

using namespace std;
class Sphere : public Mesh
{
private:
	int nverts;
	GLuint vao;
	vector<GLushort> indices;
public:
	void display();
	Sphere(string id);
	~Sphere();
};

