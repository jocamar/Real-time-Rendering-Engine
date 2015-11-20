#include "SceneManager.h"

SceneNode::SceneNode(const char *id, SceneManager *manager, SceneNode *parent, Animation *anim) : AttacheableObject(manager, parent)
{
	this->id = id;
	this->animation = anim;
	this->children = vector<shared_ptr<AttacheableObject>>();
	this->caminho = 0;

	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->orientation = glm::quat();

	this->scaleOrig = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotOrig = glm::vec3(0.0f, 0.0f, 0.0f);

	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->front = glm::vec3(0.0f, 0.0f, 1.0f);
	this->right = glm::vec3(1.0f, 0.0f, 0.0f);
}



void SceneNode::display(glm::mat4 transf, char *material, Camera *camera) {
	
	auto m_pos = glm::translate(glm::mat4(), position);
	auto m_scale = glm::scale(glm::mat4(), scale);
	auto m_rot = glm::mat4_cast(orientation);

	auto m_scale_orig = glm::translate(glm::mat4(), -scaleOrig);
	auto m_rot_orig = glm::translate(glm::mat4(), -rotOrig);
	auto m_scale_orig_inv = glm::translate(glm::mat4(), scaleOrig);
	auto m_rot_orig_inv = glm::translate(glm::mat4(), rotOrig);

	auto modelMatrix = m_pos *(m_rot_orig_inv * m_rot * m_rot_orig) * (m_scale_orig_inv * m_scale * m_scale_orig);
	modelMatrix = transf * modelMatrix;
	this->transfMatrix = modelMatrix;

	char *mat;
	if (this->material)
		mat = this->material;
	else mat = material;

	for(auto child : children)
	{
		child->display(modelMatrix, mat, camera);
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

void SceneNode::applyAnimationTranslation()
{
	if (animation != NULL)
		animation->applyTranslations(this);
}

void SceneNode::applyAnimationRotation()
{
	if (animation != NULL)
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


void SceneNode::Update(float seconds) {
	if (animation!=NULL)
	{ 
		animation->update(seconds, false);
		animation->applyTranslations(this);
		animation->applyRotations(this);
	}
	
	for (int x = 0; x < children.size(); x++)
	{
		children.at(x)->Update(seconds);
	}
}



shared_ptr<SceneNode> SceneNode::createNewChildNode(char* id)
{
	return this->createNewChildNode(id, nullptr, glm::vec3(0.0f, 0.0f, 0.0f));
}



shared_ptr<SceneNode> SceneNode::createNewChildNode(char* id, char *material, glm::vec3 position, glm::quat orientation, glm::vec3 scale)
{
	auto nodePtr = new SceneNode(id, this->manager);
	nodePtr->setMaterial(material);
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
