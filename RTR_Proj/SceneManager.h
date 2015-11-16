#pragma once
#include "SceneNode.h"
#include "Shader.h"

#include <map>

class SceneManager
{
protected:
	unique_ptr<SceneNode> root;
	map<const char*, Shader> shaders;
	char* defaultShader;

public:
	SceneManager();
	void addShader(char* id, char* vert, char* frag);
	void setDefaultShader(char* id);
	const char* getDefaultShaderId();
	Shader* getShader(const char* shader);

	shared_ptr<Entity> createEntity(char* id, char* mesh);
	shared_ptr<Entity> createEntity(char* id, Mesh* mesh);

	SceneNode* getRoot();

	void render(Camera *camera);
};