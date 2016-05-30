#include "Scene Graph/Entity.h"
#include "Scene Graph/SceneManager.h"
#include "Camera.h"

Entity::Entity(const char *idEntity, SceneManager *manager, const char *modelId, SceneNode *parent, bool shadowCaster, bool transparent) : AttacheableObject(idEntity, manager, parent)
{
	if(modelId)
	{
		this->model = manager->getModelNum(modelId);
		auto meshes = manager->getModel(model)->getMeshes();

		for(auto m : meshes)
		{
			SubEntity se;
			se.model = model;
			se.entity = this;
			se.material = -1;
			se.mesh = m;

			this->subEntities.push_back(se);
		}
	}
	else {
		this->model = -1;
	}

	this->shadowCaster = shadowCaster;
	this->transparent = transparent;
}



void Entity::display(int material, Camera *camera, bool shadowMap, Globals::LIGHT_TYPE shadowType) {

	if (shadowMap && !this->shadowCaster)
		return;

	for(auto se : subEntities)
	{
		auto transf = this->getParent()->getTransfMatrix();
		se.mesh->display(transf, material, camera, shadowMap, shadowType);
	}
}



bool Entity::isLeaf()
{
	return true;
}



void Entity::update(float seconds)
{
}

void Entity::setMaterialToUse(const char* material)
{
	auto mat = manager->getMaterialNum(material);
	for (int i = 0; i < subEntities.size(); i++)
	{
		auto se = &(subEntities[i]);
		se->material = mat;
		se->materialToUse = mat;
	}
}


RenderOrder Entity::getRenderEntities(int material, Camera *camera, bool shadowMap, Globals::LIGHT_TYPE shadowType)
{
	RenderOrder order;

	if (shadowMap && !this->shadowCaster)
		return order;

	for (int j = 0; j < subEntities.size(); j++)
	{
		SubEntity *entity = &(subEntities[j]);
		int materialToUse;
		
		if (entity->material >= 0)
			materialToUse = entity->material;
		else if (entity->mesh->getMaterialId() >= 0)
			materialToUse = entity->mesh->getMaterialId();
		else if (this->material >= 0)
			materialToUse = this->material;
		else
			materialToUse = material;
			

		entity->materialToUse = materialToUse;

		Material *mat = manager->getMaterial(materialToUse);
		if (mat->isTransparent())
			continue;

		auto omniCam = false;
		if (shadowMap && shadowType == Globals::POINT)
			omniCam = true;

		if(entity->isInFrustum(camera, omniCam))
			order.Entities.push_back(entity);
	}

	return order;
}



bool Entity::isTransparent()
{
	return transparent;
}



bool SubEntity::isInFrustum(Camera* camera, bool omniCam)
{
	glm::mat4 modelMatrix = this->entity->getParent()->getTransfMatrix();
	auto camPos = camera->getPosition();

	auto radius = glm::length((modelMatrix * glm::vec4(this->mesh->getBoundingBox().points[0],1.0)) - (modelMatrix * glm::vec4(this->mesh->getBoundingBox().center,1.0)));
	auto center = this->mesh->getBoundingBox().center;

	glm::vec3 pos = glm::vec3(modelMatrix * glm::vec4(center, 1.0));
	auto v = pos - camPos;

	if (!omniCam)
	{
		auto Z = glm::normalize(camera->getFront());

		auto pointZ = glm::dot(v, Z);
		if (pointZ > camera->Far + radius || pointZ < camera->Near - radius)
			return false;

		auto Y = glm::normalize(camera->getUp());
		auto X = glm::normalize(camera->getRight());
		auto pointY = glm::dot(v, Y);
		auto pointX = glm::dot(v, X);

		if(!camera->Ortho)
		{
			auto d = radius * camera->yFactor;
			pointZ *= glm::tan(glm::radians(camera->Zoom/2.0));

			if (pointY > pointZ + d || pointY < -pointZ - d)
				return false;
		
			auto d2 = radius * camera->xFactor;
			pointZ *= camera->Ratio;

			if (pointX > pointZ + d2 || pointX < -pointZ -d2)
				return false;
		}
		else
		{
			if (-30.0 > pointY + radius || pointY - radius > 30.0)
				return false;
			if (-30.0 > pointX + radius || pointX - radius > 30.0)
				return false;
		}
	}
	else
	{
		for (int i = 0; i < 6; i++)
		{
			glm::vec3 Z;
			glm::vec3 Y;
			glm::vec3 X;

			if (i == 0)
			{
				Z = glm::vec3(1.0, 0.0, 0.0);
				Y = glm::vec3(0.0, -1.0, 0.0);
				X = glm::vec3(0.0, 0.0, -1.0);
			}
			else if (i == 1)
			{
				Z = glm::vec3(-1.0, 0.0, 0.0);
				Y = glm::vec3(0.0, -1.0, 0.0);
				X = glm::vec3(0.0, 0.0, 1.0);
			}
			else if (i == 2)
			{
				Z = glm::vec3(0.0, 1.0, 0.0);
				Y = glm::vec3(0.0, 0.0, 1.0);
				X = glm::vec3(1.0, 0.0, 0.0);
			}
			else if (i == 3)
			{
				Z = glm::vec3(0.0, -1.0, 0.0);
				Y = glm::vec3(0.0, 0.0, -1.0);
				X = glm::vec3(1.0, 0.0, 0.0);
			}
			else if (i == 4)
			{
				Z = glm::vec3(0.0, 0.0, 1.0);
				Y = glm::vec3(0.0, -1.0, 0.0);
				X = glm::vec3(1.0, 0.0, 0.0);
			}
			else if (i == 5)
			{
				Z = glm::vec3(0.0, 0.0, -1.0);
				Y = glm::vec3(0.0, -1.0, 0.0);
				X = glm::vec3(-1.0, 0.0, 0.0);
			}

			auto pointZ = glm::dot(v, Z);
			if (pointZ - radius > camera->Far || pointZ + radius < camera->Near)
				continue;

			auto pointY = glm::dot(v, Y);
			auto pointX = glm::dot(v, X);

			if (!camera->Ortho)
			{
				auto d = radius * camera->yFactor;
				pointZ *= glm::tan(glm::radians(camera->Zoom/2.0));

				if (pointY > pointZ + d || pointY < -pointZ - d)
					continue;

				auto d2 = radius * camera->xFactor;
				pointZ *= camera->Ratio;

				if (pointX > pointZ + d2 || pointX < -pointZ - d2)
					continue;
			}
			else
			{
				if (-50.0 > pointY + radius || pointY - radius > 50.0)
					continue;
				if (-50.0 > pointX + radius || pointX - radius > 50.0)
					continue;
			}

			return true;
		}

		return false;
	}

	return true;
}



float SubEntity::distanceToCamera(Camera* camera) const
{
	glm::vec3 cam_pos = camera->getPosition();
	glm::vec3 this_pos = this->entity->getParent()->getWorldPosition();

	return glm::distance(cam_pos, this_pos);
}



bool SubEntity::operator<(const SubEntity& se) const
{
	auto manager = this->entity->getManager();
	auto mat1 = manager->getMaterial(this->materialToUse);
	auto mat2 = manager->getMaterial(se.materialToUse);
	bool trans1 = false;
	bool trans2 = false;

	if (mat1)
	{
		trans1 = (mat1->isTransparent() || this->entity->isTransparent());
	}
	if (mat2)
	{
		trans2 = (mat2->isTransparent() || se.entity->isTransparent());
	}

	if (trans1 && !trans2)
		return false;
	else if (trans2 && !trans1)
		return true;
	else return (this->materialToUse < se.materialToUse);
}

bool SubEntity::less(SubEntity *se1, SubEntity *se2)
{
	auto manager = se1->entity->getManager();
	auto mat1 = manager->getMaterial(se1->materialToUse);
	auto mat2 = manager->getMaterial(se2->materialToUse);
	bool trans1 = true;
	bool trans2 = false;

	if (mat1)
	{
		trans1 = (mat1->isTransparent() || se1->entity->isTransparent());
	}
	if (mat2) 
	{
		trans2 = (mat2->isTransparent() || se2->entity->isTransparent());
	}

	if (trans1 && !trans2)
		return false;
	else if (trans2 && !trans1)
		return true;
	else return (se1->materialToUse < se2->materialToUse);
}