#pragma once

#include "AttacheableObject.h"

class Mesh;

using namespace std;
class Light : public AttacheableObject
{
private:
	string idEntity;
	Mesh *EntityMesh;

	GLfloat *ambient;
	GLfloat *diffuse;
	GLfloat *specular;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	GLfloat *direction;
	bool directional;

public:
	Light(string idEntity, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, Mesh *EntityMesh, SceneManager *manager, SceneNode *parent = nullptr);
	Light(string idEntity, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat *direction, SceneManager *manager);
	void display(glm::mat4 transf, char *material, Camera *camera = nullptr) override;
	bool isLeaf() override;
	bool isDirectional();

	GLfloat* getAmbient();
	GLfloat* getDiffuse();
	GLfloat* getSpecular();
	GLfloat* getDirection();

	GLfloat getConstant();
	GLfloat getLinear();
	GLfloat getQuadratic();
};