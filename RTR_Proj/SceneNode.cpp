#include "SceneManager.h"

SceneNode::SceneNode(const char *id, SceneManager *manager, SceneNode *parent, Animation *anim) : AttacheableObject(id, manager, parent)
{
	this->animation = anim;
	this->children = vector<AttacheableObject*>();

	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->orientation = glm::quat();

	this->scaleOrig = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotOrig = glm::vec3(0.0f, 0.0f, 0.0f);

	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->right = glm::vec3(1.0f, 0.0f, 0.0f);
}



void SceneNode::display(glm::mat4 transf, int material, Camera *camera, bool shadowMap, Globals::LIGHT_TYPE shadowType) {
	
	/*auto m_pos = glm::translate(glm::mat4(), position);
	auto m_scale = glm::scale(glm::mat4(), scale);
	auto m_rot = glm::mat4_cast(orientation);

	auto m_scale_orig = glm::translate(glm::mat4(), -scaleOrig);
	auto m_rot_orig = glm::translate(glm::mat4(), -rotOrig);
	auto m_scale_orig_inv = glm::translate(glm::mat4(), scaleOrig);
	auto m_rot_orig_inv = glm::translate(glm::mat4(), rotOrig);

	auto modelMatrix = m_pos *(m_rot_orig_inv * m_rot * m_rot_orig) * (m_scale_orig_inv * m_scale * m_scale_orig);
	modelMatrix = transf * modelMatrix;
	this->transfMatrix = modelMatrix;*/

	int mat;
	if (this->material >= 0)
		mat = this->material;
	else mat = material;

	for(auto child : children)
	{
		child->display(this->transfMatrix, mat, camera, shadowMap, shadowType);
	}
	
}



void SceneNode::attach(AttacheableObject *object)
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



void SceneNode::applyAnimationTranslation()
{
	if (animation != nullptr)
		animation->applyTranslations(this);
}



void SceneNode::applyAnimationRotation()
{
	if (animation != nullptr)
		animation->applyRotations(this);
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



glm::vec3 SceneNode::getWorldPosition()
{
	return glm::vec3(transfMatrix[3]);
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

void SceneNode::setTransfMatrix(glm::mat4 transf)
{
	this->transfMatrix = transf;
}


void SceneNode::setAnimation(Animation *anim)
{
	this->animation = anim;
}


void SceneNode::update(float millis) {
	if (animation!=nullptr)
	{ 
		animation->update(millis, false);
		animation->applyTranslations(this);
		animation->applyRotations(this);
	}

	auto m_pos = glm::translate(glm::mat4(), position);
	auto m_scale = glm::scale(glm::mat4(), scale);
	auto m_rot = glm::mat4_cast(orientation);

	auto m_scale_orig = glm::translate(glm::mat4(), -scaleOrig);
	auto m_rot_orig = glm::translate(glm::mat4(), -rotOrig);
	auto m_scale_orig_inv = glm::translate(glm::mat4(), scaleOrig);
	auto m_rot_orig_inv = glm::translate(glm::mat4(), rotOrig);

	auto modelMatrix = m_pos *(m_rot_orig_inv * m_rot * m_rot_orig) * (m_scale_orig_inv * m_scale * m_scale_orig);
	if(this->parent)
		modelMatrix = this->parent->getTransfMatrix() * modelMatrix;
	this->transfMatrix = modelMatrix;
	
	for (int x = 0; x < children.size(); x++)
	{
		children.at(x)->update(millis);
	}
}



SceneNode* SceneNode::createNewChildNode(char* id)
{
	return this->createNewChildNode(id, nullptr, glm::vec3(0.0f, 0.0f, 0.0f));
}



SceneNode* SceneNode::createNewChildNode(char* id, char *material, glm::vec3 position, glm::quat orientation, glm::vec3 scale)
{
	auto nodePtr = new SceneNode(id, this->manager);
	nodePtr->setMaterial(material);
	nodePtr->translate(position);
	nodePtr->changeOrientation(orientation);
	nodePtr->changeScale(scale);

	this->attach(nodePtr);
	return nodePtr;
}



SceneNode::~SceneNode(void)
{
}
