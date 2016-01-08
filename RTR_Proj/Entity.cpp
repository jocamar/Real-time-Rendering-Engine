#include "Entity.h"
#include "SceneManager.h"
#include "Camera.h"

Entity::Entity(const char *idEntity, SceneManager *manager, const char *modelId, SceneNode *parent, bool shadowCaster) : AttacheableObject(idEntity, manager, parent) 
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

		Material *mat = manager->getMaterial(materialToUse);
		Shader *shader = mat->getShader();

		order.Entities[shader][mat].push_back(entity);
	}

	return order;
}



bool SubEntity::isInFrustum(Camera* camera)
{
	return true;
}



float SubEntity::distanceToCamera(Camera* camera)
{
	glm::vec3 cam_pos = camera->Position;
	glm::vec3 this_pos = this->entity->getParent()->getWorldPosition();

	return glm::distance(cam_pos, this_pos);
}