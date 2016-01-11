#pragma once

#include <iostream>
#include "Mesh.h"

using namespace std;
class Skybox : public Mesh
{
	GLuint cubemapTexture;
public:
	Skybox(const char *id, SceneManager *manager);
	void display(glm::mat4 transf, int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;
	GLuint loadCubemap(vector<const GLchar*> faces);
	~Skybox();
};

