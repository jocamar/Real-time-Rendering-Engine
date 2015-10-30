#pragma once

#include <GL/glew.h>
#include <iostream>

using namespace std;
class Mesh
{
protected:
	GLuint VAO;
	string idMesh;
public:
	GLuint getVAO();
	virtual void display();
};

