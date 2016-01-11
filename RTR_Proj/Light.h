#pragma once

#include "AttacheableObject.h"
#include "Entity.h"

class Mesh;

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const GLuint DIR_SHADOW_WIDTH = 1024, DIR_SHADOW_HEIGHT = 1024;

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

	GLuint depthMapFBO;
	GLuint depthMap;
	GLuint depthCubemap;
	Camera *cam;
	glm::mat4 lightSpaceMatrix;
	std::vector<glm::mat4> cubeLightSpaceMatrixes;

public:
	Light(const char *idEntity, SceneManager *manager, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, const char *modelId, SceneNode *parent = nullptr);
	Light(const char *idEntity, SceneManager *manager, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat *direction);
	void display(int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;
	bool isLeaf() override;
	void update(float seconds) override {};
	void generateShadowMap(Camera *camera);
	bool isDirectional();

	GLfloat* getAmbient();
	GLfloat* getDiffuse();
	GLfloat* getSpecular();
	GLfloat* getDirection();
	glm::mat4* getLightSpaceMatrix();
	GLuint getShadowMap();
	GLuint getCubeShadowMap();

	GLfloat getConstant();
	GLfloat getLinear();
	GLfloat getQuadratic();
};