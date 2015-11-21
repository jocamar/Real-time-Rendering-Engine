#pragma once

#include "Globals.h"

#include <vector>
#include <iostream>

class SceneNode;
using std::vector;
using std::cout;
using std::endl;

class Animation {
protected:
	vector<glm::vec3> ctrlPoints;
public:
	Animation(vector<glm::vec3> ctrlPoints);
	virtual void update(unsigned long milis, bool animationsPaused) = 0;
	virtual void applyTranslations(SceneNode *node) = 0;
	virtual void applyRotations(SceneNode *node) = 0;
	virtual void reset() = 0;
};

class LinearAnimation : public Animation {
	glm::vec3 curPos;
	glm::vec3 prevPos;
	float curOrientation;
	float prevOrientation;
	glm::vec3 curDirection;
	int curPoint;
	float speed;
	bool looping;
public:
	bool finished;
	LinearAnimation(vector<glm::vec3> ctrlPoints, float span, bool looping = false);
	void update(unsigned long milis, bool animationsPaused = false) override;
	void applyTranslations(SceneNode *node) override;
	void applyRotations(SceneNode *node) override;
	void reset() override;
	glm::vec3 getPos() const;
};

