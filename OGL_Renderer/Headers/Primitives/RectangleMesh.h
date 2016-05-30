#pragma once

#include <iostream>
#include "Mesh.h"

using namespace std;

/**
* Class representing a 1x1 square mesh subdivided into a number of quads on the scene.
*/
class RectangleMesh : public Mesh
{
	int parts;
public:

	/**
	* Rectangle mesh constructor.
	*
	* @param id is a unique primitive ID used to retrieve the object.
	* @param manager is the scene manager to which this object is assigned.
	*/
	RectangleMesh(const char *id, SceneManager *manager, int parts, float t_x = 1.0f, float t_y = 1.0f);

	/**
	* Displays the rectangle mesh.
	*
	* @param transf is the object's model matrix.
	* @param material is the object's material id number.
	* @param camera is the current camera used to render.
	* @param shadowMap is a boolean indicating whether the object is being rendered for generating the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated, directional or omni-directional.
	*/
	void display(glm::mat4 transf, int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;

	/**
	* Rectangle mesh destructor.
	*/
	~RectangleMesh();
};