#pragma once

#include "Globals.h"

#include <vector>
#include <iostream>

class SceneNode;
using std::vector;
using std::cout;
using std::endl;

/**
* Class representing an animation to be applied to a node in the scene graph.
*/
class Animation {
protected:
	//Control points for the animation.
	vector<glm::vec3> ctrlPoints;
public:
	/**
	* Animation constructor.
	*/
	Animation();

	/**
	* Animation constructor.
	*
	* @param ctrlPoints specifies the control points of the animation.
	*/
	Animation(vector<glm::vec3> ctrlPoints);

	/**
	* Updates the animation every frame.
	*
	* @param millis is the number of milliseconds elapsed since the last frame.
	* @param animationsPaused specifies whether the animation is paused.
	*/
	virtual void update(unsigned long milis, bool animationsPaused) = 0;

	/**
	* Applies the translation of this animation to the specified node, updating its position according to the delta value since the last update.
	*
	* @param node is the node to update.
	*/
	virtual void applyTranslations(SceneNode *node) = 0;

	/**
	* Applies the rotations of this animation to the specified node, updating its orientation according to the delta value since the last update.
	*
	* @param node is the node to update.
	*/
	virtual void applyRotations(SceneNode *node) = 0;

	/**
	* Activates and deactivates the node according to the animation.
	*
	* @param node is the node to update.
	*/
	virtual void applyActive(SceneNode *node) {};

	/**
	* Resets the animation.
	*/
	virtual void reset() = 0;
};

/**
* Class representing a linear animation where the node moves linearly from control point to control point with a certain speed.
*/
class LinearAnimation : public Animation {
	//Current position of the animation.
	glm::vec3 curPos;

	//Previous position of the animation.
	glm::vec3 prevPos;

	//Current orientation of the animation.
	float curOrientation;

	//Previous orientation of the animation.
	float prevOrientation;

	//Current direction of the movement.
	glm::vec3 curDirection;

	//Current control point to which the animation is moving towards.
	int curPoint;

	//Animation speed.
	float speed;

	//Animation looping.
	bool looping;
public:

	//Animation is finished.
	bool finished;

	/**
	* Linear animation constructor.
	*
	* @param ctrlPoints specifies the control points for the animation.
	* @param span specifies the amount of time in milliseconds that the animation lasts.
	* @param looping specifies whether the animation loops once it's finished.
	*/
	LinearAnimation(vector<glm::vec3> ctrlPoints, float span, bool looping = false);

	/**
	* Updates the animation every frame.
	*
	* @param millis is the number of milliseconds elapsed since the last frame.
	* @param animationsPaused specifies whether the animation is paused.
	*/
	void update(unsigned long milis, bool animationsPaused = false) override;

	/**
	* Applies the translation of this animation to the specified node, updating its position according to the delta value since the last update.
	*
	* @param node is the node to update.
	*/
	void applyTranslations(SceneNode *node) override;

	/**
	* Applies the rotations of this animation to the specified node, updating its orientation according to the delta value since the last update.
	*
	* @param node is the node to update.
	*/
	void applyRotations(SceneNode *node) override;

	/**
	* Resets the animation.
	*/
	void reset() override;

	/**
	* Returns the current position of the animation.
	*/
	glm::vec3 getPos() const;
};


/**
* Class representing an improved animation where the node changes its yaw, pitch, roll and x, y, z positions linnearly between sets of control points.
* Other values can later be added that can change as well such as color intensity, scale and opacity.
*/
class BetterAnimation : public Animation {
	//Total time in milliseconds since the start of the animation.
	long totalTime;

	//Control point vectors for position.
	vector<std::pair<long, float>> pos_xs;
	vector<std::pair<long, float>> pos_ys;
	vector<std::pair<long, float>> pos_zs;

	//Current control point indexes for position.
	int pos_x_Ind, pos_y_Ind, pos_z_Ind;

	//Current position.
	float currX, currY, currZ;

	//Control point vectors for orientation.
	vector<std::pair<long, float>> yaws;
	vector<std::pair<long, float>> pitches;
	vector<std::pair<long, float>> rolls;

	//Current control poinr indexes for orientation.
	int yaws_Ind, pitches_Ind, rolls_Ind;

	//Current orientation.
	float currYaw, currPitch, currRoll;

	//Control points for activating and deactivating the node.
	vector<std::pair<long, bool>> actives;
	int active_Ind;
	float currActive;

	bool looping;
	bool started;

	//Previous position of the animation.
	glm::vec3 prevPos;

	//Previous orientation of the animation.
	float prevYaw, prevRoll, prevPitch;
public:
	enum Attribute {POS_X, POS_Y, POS_Z, YAW, PITCH, ROLL, ACTIVE};

	//Animation is finished.
	bool finished;

	/**
	* Better animation constructor.
	*
	* @param looping specifies whether the animation loops when it's finished.
	*/
	BetterAnimation(bool looping = false);

	/**
	* Updates the animation every frame.
	*
	* @param millis is the number of milliseconds elapsed since the last frame.
	* @param animationsPaused specifies whether the animation is paused.
	*/
	void update(unsigned long milis, bool animationsPaused = false) override;

	/**
	* Applies the translation of this animation to the specified node, updating its position according to the delta value since the last update.
	*
	* @param node is the node to update.
	*/
	void applyTranslations(SceneNode *node) override;

	/**
	* Applies the rotations of this animation to the specified node, updating its orientation according to the delta value since the last update.
	*
	* @param node is the node to update.
	*/
	void applyRotations(SceneNode *node) override;

	/**
	* Activates and deactivates the node according to the animation.
	*
	* @param node is the node to update.
	*/
	void applyActive(SceneNode *node) override;

	/**
	* Resets the animation.
	*/
	void reset() override;

	/**
	* Returns the current position of the animation.
	*/
	glm::vec3 getPos() const;

	/**
	* Adds the specified control points to the animation.
	*
	* @param ctrlPoints specified the control points and values to add to the animation.
	* @param type specifies what type of control points are being added.
	*/
	void addControlPoints(vector<std::pair<long, float>> ctrlPoints, Attribute type);

	/**
	* Starts the animation.
	*/
	void start();
};

