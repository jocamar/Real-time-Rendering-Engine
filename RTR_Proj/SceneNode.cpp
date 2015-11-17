#include "SceneManager.h"
#include <glm/gtc/matrix_transform.hpp>

SceneNode::SceneNode(const char *id, SceneManager *manager, SceneNode *parent) : AttacheableObject(manager, parent)
{
	this->id = id;
	this->animation = nullptr;
	this->children = vector<shared_ptr<AttacheableObject>>();

	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->orientation = glm::quat();

	this->scaleOrig = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotOrig = glm::vec3(0.0f, 0.0f, 0.0f);

	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->right = glm::vec3(1.0f, 0.0f, 0.0f);
}



SceneNode::SceneNode(const char *id, shared_ptr<Entity> entity, char *animRef, SceneManager *manager, SceneNode *parent) : AttacheableObject(manager, parent)
{
	this->id = id;
	this->animation = animRef;
	this->children = vector<shared_ptr<AttacheableObject>>();
	this->children.push_back(entity);

	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->orientation = glm::quat();

	this->scaleOrig = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotOrig = glm::vec3(0.0f, 0.0f, 0.0f);
}



void SceneNode::display(glm::mat4 transf, char *shader, Camera *camera) {
	
	auto m_pos = glm::translate(glm::mat4(), position);
	auto m_scale = glm::scale(glm::mat4(), scale);
	auto m_rot = glm::mat4_cast(orientation);

	auto m_scale_orig = glm::translate(glm::mat4(), -scaleOrig);
	auto m_rot_orig = glm::translate(glm::mat4(), -rotOrig);
	auto m_scale_orig_inv = glm::translate(glm::mat4(), scaleOrig);
	auto m_rot_orig_inv = glm::translate(glm::mat4(), rotOrig);

	auto modelMatrix = m_pos * (m_rot_orig_inv * m_rot * m_rot_orig) * (m_scale_orig_inv * m_scale * m_scale_orig);
	modelMatrix = transf * modelMatrix;
	
	for(auto child : children)
	{
		child->display(modelMatrix, shader, camera);
	}
	
}

void SceneNode::attach(shared_ptr<AttacheableObject> object)
{
	object->setParent(this);
	this->children.push_back(object);
}



bool SceneNode::isLeaf()
{
	if (children.empty())
		return true;
	return false;
}



glm::vec3 SceneNode::getPosition()
{
	return position;
}



glm::vec3 SceneNode::getScale()
{
	return scale;
}



glm::quat SceneNode::getOrientationQuaternion()
{
	return orientation;
}



glm::vec3 SceneNode::getScaleOrig()
{
	return scaleOrig;
}



glm::vec3 SceneNode::getRotOrig()
{
	return rotOrig;
}



glm::mat4 SceneNode::getTransfMatrix()
{
	return this->transfMatrix;
}



void SceneNode::translate(glm::vec3 translation)
{
	this->position += translation;
}



void SceneNode::changeScale(glm::vec3 scale)
{
	this->scale *= scale;
}



void SceneNode::setScale(glm::vec3 scale)
{
	this->scale = scale;
}



void SceneNode::changeOrientation(glm::quat quaternion)
{
}


void SceneNode::yaw(GLfloat degrees)
{
	auto rot = glm::angleAxis(glm::radians(degrees), up);

	this->right = rot * right;
	this->front = rot * front;
	this->orientation = rot * orientation;
}



void SceneNode::pitch(GLfloat degrees)
{
	auto rot = glm::angleAxis(glm::radians(degrees), right);

	this->up = rot * up;
	this->front = rot * front;
	this->orientation = rot * orientation;
}



void SceneNode::roll(GLfloat degrees)
{
	auto rot = glm::angleAxis(glm::radians(degrees), front);

	this->up = rot * up;
	this->right = rot * right;
	this->orientation = rot * orientation;
}



void SceneNode::setScaleOrig(glm::vec3 orig)
{
	this->scaleOrig = orig;
}



void SceneNode::setRotOrig(glm::vec3 orig)
{
	this->rotOrig = orig;
}



shared_ptr<SceneNode> SceneNode::createNewChildNode(char* id)
{
	return this->createNewChildNode(id, glm::vec3(0.0f, 0.0f, 0.0f));
}



shared_ptr<SceneNode> SceneNode::createNewChildNode(char* id, glm::vec3 position, glm::quat orientation, glm::vec3 scale)
{
	auto nodePtr = new SceneNode(id, this->manager);
	nodePtr->translate(position);
	nodePtr->changeOrientation(orientation);
	nodePtr->changeScale(scale);

	shared_ptr<SceneNode> node(nodePtr);

	this->attach(shared_ptr<AttacheableObject>(node));
	return node;
}



SceneNode::~SceneNode(void)
{
}
