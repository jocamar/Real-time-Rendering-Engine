#pragma once

#include <iostream>
#include "Mesh.h"

using namespace std;
class RectangleMesh : public Mesh
{
private:
	int parts;
public:
	void display() override;
	RectangleMesh(string id, int parts, float t_x = 1.0f, float t_y = 1.0f);
	~RectangleMesh();
};