#pragma once

#include "Globals.h"

#include <vector>
#include "AttacheableObject.h"
#include "Animation.h"

class SceneManager;

using namespace std;

/**
* Class representing a node in the scene graph. A node is not rendered but can apply transformations, animations and materials to entities, lights and child nodes attached to it.
*/
class SceneNode : public AttacheableObject
{
protected:
	//Animation attached to this node.
	Animation *animation;

	//The node's children.
	vector<AttacheableObject*> children;

	glm::vec3 position;
	glm::vec3 scale;

	//Node orientation.
	glm::quat orientation;
	float curr_yaw, curr_pitch, curr_roll;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;

	//Node scale and rotation origin to be used when applying scaling and rotation operations.
	glm::vec3 scaleOrig;
	glm::vec3 rotOrig;

	//Node Model matrix.
	glm::mat4 transfMatrix;
	bool calculateMatrix;

public:

	/**
	* Scene node constructor.
	*
	* @param id is the unique ID of the node.
	* @param manager is the scene manager to which the node is assigned.
	* @parent is the parent node if this node.
	* @anim is the animation attached to this node.
	*/
	SceneNode(const char *id, SceneManager *manager, SceneNode *parent = nullptr, Animation *anim = nullptr);

	/**
	* Scene node destructor.
	*/
	~SceneNode();

	/**
	* Displays this node.
	* @TODO check whether this function is being used at all, delete if not.
	*
	* @param material is the object's material id number.
	* @param camera is the current camera used to render.
	* @param shadowMap is a boolean indicating whether the object is being rendered for generating the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated, directional or omni-directional.
	*/
	void display(int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;

	/**
	* Attaches an object to this node as a child.
	*
	* @param object is the object to attach.
	*/
	void attach(AttacheableObject *object);

	/**
	* Returns whether this object is a leaf in the graph.
	*/
	bool isLeaf() override;

	/**
	* Updates the node on each frame, updating its animation, position, orientation and model matrix.
	*/
	void update(float seconds) override;

	/**
	* Returns the entities to be rendered among the children of this node, culling any entities that are not to be rendered.
	*
	* @param material is the id number of the material to be applied for the rendering.
	* @param camera is the camera being used to render.
	* @param shadowMap is a boolean indicating whether the object is being rendered for generating the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated, directional or omni-directional.
	*/
	RenderOrder getRenderEntities(int material, Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;

	/**
	* Returns the position vector of this node.
	*/
	glm::vec3 getPosition();

	/**
	* Returns the scale vector of this node.
	*/
	glm::vec3 getScale();

	/**
	* Returns the quaternion representing the orientation of this node.
	*/
	glm::quat getOrientationQuaternion();

	/**
	* Returns the scaling origin of this node.
	*/
	glm::vec3 getScaleOrig();

	/**
	* Returns the rotation origin of this node.
	*/
	glm::vec3 getRotOrig();

	/**
	* Returns the model matrix of this node.
	*/
	glm::mat4 getTransfMatrix();

	/**
	* Returns the world position of this node.
	*/
	glm::vec3 getWorldPosition();

	/**
	* Translates this node according to the specidied vector.
	*
	* @param translation is the translation to apply to the node.
	*/
	void translate(glm::vec3 translation);

	/**
	* Scales the node according to the specified scaling vector.
	*
	* @param scale is the scaling vector to use.
	*/
	void changeScale(glm::vec3 scale);

	/**
	* Sets the scale of this node.
	*
	* @param scale is the scaling vector to use.
	*/
	void setScale(glm::vec3 scale);

	/**
	* Changes the orientation of this node according to the specified quaternion.
	*
	* @param quaternion is the quaternion representing the rotation to apply to this node.
	*/
	void changeOrientation(glm::quat quaternion);

	/**
	* Changes the yaw of this node the specified amount of degrees.
	*
	* @param degrees is the amount of degrees to rotate the node.
	*/
	void yaw(GLfloat degrees);

	/**
	* Changes the pitch of this node the specified amount of degrees.
	*
	* @param degrees is the amount of degrees to rotate the node.
	*/
	void pitch(GLfloat degrees);

	/**
	* Changes the roll of this node the specified amount of degrees.
	*
	* @param degrees is the amount of degrees to rotate the node.
	*/
	void roll(GLfloat degrees);

	/**
	* Sets the scaling origin of this node.
	*
	* @param orig is the new scaling origin.
	*/
	void setScaleOrig(glm::vec3 orig);

	/**
	* Sets the rotation origin of this node.
	*
	* @param orig is the new rotation origin.
	*/
	void setRotOrig(glm::vec3 orig);

	/**
	* Sets the model matrix of this node.
	*
	* @param transf is the new model matrix.
	*/
	void setTransfMatrix(glm::mat4 transf);

	/**
	* Sets the animation of this node.
	*
	* @param anim is the new animation for this node.
	*/
	void setAnimation(Animation *anim);

	/**
	* Applies the translation of the animation to this node, updating its position.
	*/
	void applyAnimationTranslation();

	/**
	* Applies the rotation of the animation to this node, updating its rotation.
	*/
	void applyAnimationRotation();

	/**
	* Creates a new child node attached to this node and returns it.
	*
	* @param id is the unique ID of the node being created.
	*/
	SceneNode* createNewChildNode(char *id);

	/**
	* Creates a new child node attached to this node and returns it.
	*
	* @param id is the unique ID of the node being created.
	* @param material is the ID of the material to use.
	* @param position is the position of this node (relative to its origin).
	* @param orientation is the orientation of this node (relative to its origin).
	* @param scale is the scale of this node (relative to its origin).
	*/
	SceneNode* createNewChildNode(char *id, char *material, glm::vec3 position, glm::quat orientation = glm::quat(), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};

