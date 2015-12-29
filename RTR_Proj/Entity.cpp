#include "Entity.h"
#include "SceneManager.h"
#include "Camera.h"

Entity::Entity(const char *idEntity, SceneManager *manager, const char *modelId, SceneNode *parent) : AttacheableObject(idEntity, manager, parent) 
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
}



void Entity::display(glm::mat4 transf, int material, Camera *camera) {

	for(auto se : subEntities)
	{
		se.mesh->display(transf, material, camera);
	}
}



bool Entity::isLeaf()
{
	return true;
}
