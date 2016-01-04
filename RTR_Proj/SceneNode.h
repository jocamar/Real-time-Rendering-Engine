#pragma once

#include "Globals.h"

#include <vector>
#include "AttacheableObject.h"
#include "Animation.h"

class SceneManager;

using namespace std;
class SceneNode : public AttacheableObject
{
protected:
	Animation *animation;

	vector<AttacheableObject*> children;

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
	SceneNode(const char *id, SceneManager *manager, SceneNode *parent = nullptr, Animation *anim = nullptr);
	~SceneNode();

	void display(glm::mat4 transf, int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;
	void attach(AttacheableObject *object);
	bool isLeaf() override;
	void update(float seconds) override;

	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::quat getOrientationQuaternion();
	glm::vec3 getScaleOrig();
	glm::vec3 getRotOrig();

	glm::mat4 getTransfMatrix();
	glm::vec3 getWorldPosition();

	void translate(glm::vec3 translation);
	void changeScale(glm::vec3 scale);
	void setScale(glm::vec3 scale);
	void changeOrientation(glm::quat quaternion);
	void yaw(GLfloat degrees);
	void pitch(GLfloat degrees);
	void roll(GLfloat degrees);
	void setScaleOrig(glm::vec3 orig);
	void setRotOrig(glm::vec3 orig);
	void setTransfMatrix(glm::mat4 transf);
	void setAnimation(Animation *anim);
	void applyAnimationTranslation();
	void applyAnimationRotation();


	SceneNode* createNewChildNode(char *id);
	SceneNode* createNewChildNode(char *id, char *material, glm::vec3 position, glm::quat orientation = glm::quat(), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};

