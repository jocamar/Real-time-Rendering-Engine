#pragma once

#include "AttacheableObject.h"

class Mesh;

using namespace std;

class Entity;

/**
* Class representing a sub-entity. A sub-entity holds a single mesh of an entity and its material and it has a 1-to-1 correspondence with the meshes on a model.
*/
struct SubEntity
{
	//The mesh attached to this sub-entity.
	Mesh *mesh;

	//The material attached to this sub-entity.
	//@TODO check if this is being used, and use it if the mesh material is being used instead.
	int material;

	//The material to use during rendering, not necessarily the one attached to this sub-entity as that can be overriden.
	int materialToUse;

	//Distance to the camera.
	float distanceToCam;

	//ID number of the model to which the mesh belongs.
	int model;

	//Entity to which this sub-entity belongs to.
	Entity *entity;

	/**
	* Calculates whether this sub-entity is in the specified camera frustum.
	* @TODO change calculation to use the bounding box instead of the bounding sphere.
	*
	* @param camera is the camera holding the frustum.
	* @param omniCam represents whether the camera is omni-directional or not (in most cases it is not).
	*/
	bool isInFrustum(Camera *camera, bool omniCam = false);

	/**
	* Calculates the distance that this object is from the camera, using the world position of the camera and the origin of the parent.
	* @TODO change calculation to take into account the center of the mesh.
	*
	* @param camera is the camera to calculate the distance to.
	*/
	float distanceToCamera(Camera *camera) const;

	/**
	* Less-than operator, compares this sub-entity to another sub-entity according to its rendering order.
	* @TODO check whether this or the less function is being used to order and delete the unused one.
	*
	* @param se is the sub-entity to compare this to.
	*/
	bool operator<(const SubEntity& se) const;

	/*
	* Compares a sub-entity to another sub-entity according to their rendering order.
	* @TODO check whether this or the less-than operator is being used to order and delete the unused one.
	*
	* @param se1 is the first sub-entity to compare.
	* @param se1 is the second sub-entity to compare.
	*/
	static bool less(SubEntity *se1, SubEntity *se2);
};

/**
* Class representing an entity. 
* An entity is a renderable object that represents something in the scene. It is represented by a specified model and can have several sub-entities, each representing a single mesh.
* It needs to be attached to a node in the scene graph to be rendered.
*/
class Entity : public AttacheableObject
{
protected:
	int model;
	vector<SubEntity> subEntities;
	bool shadowCaster;
	bool transparent;
public:

	/*
	* Entity constructor.
	*
	* @param idEntity is the unique id of this entity.
	* @param manager is the scene manager to which this entity is assigned.
	* @param modelId is the ID of the model that represents this entity in the scene.
	* @param parent is the parent node to which this entity is attached.
	* @param shadowCaster specifies whether this entity casts shadows.
	* @param transparent specified whether this entity has transparency, forcing it to render after all opaque entities.
	*/
	Entity(const char *idEntity, SceneManager *manager, const char *modelId, SceneNode *parent = nullptr, bool shadowCaster = true, bool transparent = false);

	/**
	* Displays this entity.
	* @TODO check whether this function is being used at all, delete if not.
	*
	* @param material is the object's material id number.
	* @param camera is the current camera used to render.
	* @param shadowMap is a boolean indicating whether the object is being rendered for generating the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated, directional or omni-directional.
	*/
	void display(int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;

	/**
	* Returns whether this object is a leaf in the graph.
	*/
	bool isLeaf() override;

	/**
	* Updates the object on each cycle. Currently does nothing.
	*
	* @param seconds represents the elapsed seconds since the last update.
	* @TODO possibly change this to a long representing milliseconds.
	*/
	void update(float seconds) override;

	/**
	* Sets the material to used of the entity, overriding the default materials from the model.
	*
	* @param material is the new material to use.
	*/
	void setMaterialToUse(const char* material);

	/**
	* Returns the entities to be rendered, culling any entities that are not to be rendered.
	*
	* @param material is the id number of the material to be applied for the rendering.
	* @param camera is the camera being used to render.
	* @param shadowMap is a boolean indicating whether the object is being rendered for generating the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated, directional or omni-directional.
	*/
	RenderOrder getRenderEntities(int material, Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;

	/**
	* Returns whether this entity is transparent and must thus be rendered after opaque entities.
	*/
	bool isTransparent();
};

