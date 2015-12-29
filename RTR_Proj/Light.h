#pragma once

#include "AttacheableObject.h"
#include "Entity.h"

class Mesh;

using namespace std;
class Light : public Entity
{
	GLfloat *ambient;
	GLfloat *diffuse;
	GLfloat *specular;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	GLfloat *direction;
	bool directional;

public:
	Light(const char *idEntity, SceneManager *manager, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, const char *modelId, SceneNode *parent = nullptr);
	Light(const char *idEntity, SceneManager *manager, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat *direction);
	void display(glm::mat4 transf, int material, Camera *camera = nullptr) override;
	bool isLeaf() override;
	void update(float seconds) override {};
	bool isDirectional();

	GLfloat* getAmbient();
	GLfloat* getDiffuse();
	GLfloat* getSpecular();
	GLfloat* getDirection();

	GLfloat getConstant();
	GLfloat getLinear();
	GLfloat getQuadratic();
};