#pragma once

#include <iostream>
#include "Mesh.h"

using namespace std;
class RectangleMesh : public Mesh
{
	int parts;
public:
	RectangleMesh(const char *id, SceneManager *manager, int parts, float t_x = 1.0f, float t_y = 1.0f);
	void display(glm::mat4 transf, int material, Camera *camera = nullptr) override;
	~RectangleMesh();
};