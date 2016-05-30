#pragma once
#include "Globals.h"

class Camera;
class SceneManager;
class SceneNode;
struct SubEntity;
struct RenderOrder;

/**
* Class representing an attacheable object on the scene graph. Attacheable objects can be attached to nodes on the graph as children, inheriting materials and transforms.
*/
class AttacheableObject
{
protected:
	const char *id;
	SceneManager *manager;
	SceneNode *parent;
	int material;
public:

	/**
	* AttacheableObject constructor.
	*
	* @param id is a unique primitive ID used to retrieve the object.
	* @param manager is the scene manager to which this object is assigned.
	* @param parent is the parent node to which to attach this object.
	*/
	explicit AttacheableObject(const char *id, SceneManager *manager, SceneNode *parent = nullptr);

	/**
	* AttacheableObject constructor.
	*
	* @param id is a unique primitive ID used to retrieve the object.
	* @param manager is the scene manager to which this object is assigned.
	* @param material is the id of the material to be used by this object.
	* @param parent is the parent node to which to attach this object.
	*/
	AttacheableObject(const char *id, SceneManager *manager, char *material, SceneNode *parent = nullptr);

	/**
	* Displays this object. Must be overriden by child classes.
	*
	* @param material is the object's material id number.
	* @param camera is the current camera used to render.
	* @param shadowMap is a boolean indicating whether the object is being rendered for generating the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated, directional or omni-directional.
	*/
	virtual void display(int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) = 0;

	/**
	* Returns whether this object is a leaf in the graph. Must be overriden by child classes.
	*/
	virtual bool isLeaf() = 0;

	/**
	* Updates the object on each cycle. Must be overriden by child classes.
	*
	* @param seconds represents the elapsed seconds since the last update.
	* @TODO possibly change this to a long representing milliseconds.
	*/
	virtual void update(float seconds) = 0;

	/**
	* Returns the entities to be rendered, culling any entities that are not to be rendered. Must be overriden by child classes.
	*
	* @param material is the id number of the material to be applied for the rendering.
	* @param camera is the camera being used to render.
	* @param shadowMap is a boolean indicating whether the object is being rendered for generating the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated, directional or omni-directional.
	*/
	virtual RenderOrder getRenderEntities(int material, Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) = 0;

	/**
	* Returns the parent of this object in the scene graph.
	*/
	SceneNode* getParent();

	/**
	* Sets the parent node of this object.
	*
	* @param parent is the new parent node of this object.
	*/
	void setParent(SceneNode *parent);

	/**
	* Sets the material of this object.
	*
	* @param material is the new material to be used with this object.
	*/
	void setMaterial(char *material);

	/**
	* Returns the material of this object.
	*/
	int getMaterial();

	/**
	* Returns the scene manager to which this object is assigned.
	*/
	SceneManager* getManager();

	/**
	* Attacheable object destructor.
	*/
	virtual ~AttacheableObject() {};
};
