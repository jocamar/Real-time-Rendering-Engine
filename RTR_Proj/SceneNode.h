#pragma once

#include "Globals.h"

#include <vector>
#include <memory>

#include "Entity.h"

class SceneManager;

using namespace std;
class SceneNode : public AttacheableObject
{
protected:
	const char *id;
	char *animation;
	char *shader;
	vector<shared_ptr<AttacheableObject>> children;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;

	glm::quat orientation;

	glm::vec3 scaleOrig;
	glm::vec3 rotOrig;

	glm::mat4 transfMatrix;
	bool calculateMatrix;

public:
	SceneNode(const char *id, SceneManager *manager, SceneNode *parent = nullptr);
	SceneNode(const char *id, shared_ptr<Entity> entity, char *animRef, SceneManager *manager, SceneNode *parent = nullptr);
	~SceneNode();

	void display(glm::mat4 transf, char *shader, Camera *camera = nullptr) override;
	void attach(shared_ptr<AttacheableObject> object);
	bool isLeaf() override;

	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::quat getOrientationQuaternion();
	glm::vec3 getScaleOrig();
	glm::vec3 getRotOrig();

	glm::mat4 getTransfMatrix();

	void translate(glm::vec3 translation);
	void changeScale(glm::vec3 scale);
	void setScale(glm::vec3 scale);
	void changeOrientation(glm::quat quaternion);
	void yaw(GLfloat degrees);
	void pitch(GLfloat degrees);
	void roll(GLfloat degrees);
	void setScaleOrig(glm::vec3 orig);
	void setRotOrig(glm::vec3 orig);


	shared_ptr<SceneNode> createNewChildNode(char *id);
	shared_ptr<SceneNode> createNewChildNode(char *id, glm::vec3 position, glm::quat orientation = glm::quat(), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};
