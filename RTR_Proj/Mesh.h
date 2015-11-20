#pragma once

#include "Globals.h"

using namespace std;
class Mesh
{
protected:
	GLuint VAO;
	GLuint *buffers;
	GLuint numBuffers;
	string idMesh;
public:
	GLuint getVAO();
	virtual void display();
	virtual ~Mesh();

};

