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
	Animation();
	Animation(vector<glm::vec3> ctrlPoints);
	virtual void update(unsigned long milis, bool animationsPaused) = 0;
	virtual void applyTranslations(SceneNode *node) = 0;
	virtual void applyRotations(SceneNode *node) = 0;
	virtual void applyActive(SceneNode *node) {};
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

class BetterAnimation : public Animation {
	long totalTime;

	vector<std::pair<long, float>> pos_xs;
	vector<std::pair<long, float>> pos_ys;
	vector<std::pair<long, float>> pos_zs;
	int pos_x_Ind, pos_y_Ind, pos_z_Ind;
	float currX, currY, currZ;

	vector<std::pair<long, float>> yaws;
	vector<std::pair<long, float>> pitches;
	vector<std::pair<long, float>> rolls;
	int yaws_Ind, pitches_Ind, rolls_Ind;
	float currYaw, currPitch, currRoll;

	vector<std::pair<long, bool>> actives;
	int active_Ind;
	float currActive;

	bool looping;
	bool started;
	glm::vec3 prevPos;
	float prevYaw, prevRoll, prevPitch;
public:
	enum Attribute {POS_X, POS_Y, POS_Z, YAW, PITCH, ROLL, ACTIVE};
	bool finished;
	BetterAnimation(bool looping = false);
	void update(unsigned long milis, bool animationsPaused = false) override;
	void applyTranslations(SceneNode *node) override;
	void applyRotations(SceneNode *node) override;
	void applyActive(SceneNode *node) override;
	void reset() override;
	glm::vec3 getPos() const;
	void addControlPoints(vector<std::pair<long, float>> ctrlPoints, Attribute type);
	void start();
};

