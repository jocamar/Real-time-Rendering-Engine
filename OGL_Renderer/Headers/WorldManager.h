#pragma once

#include <map>

#include "Globals.h"
#include "Scene Graph/SceneManager.h"
#include "SceneRenderer.h"

/**
* Class representing the world manager that manages the scene, entities, renderer and media.
* @TODO actually use this...
*/
class WorldManager
{
	std::map<string,Entity*> entities;

	//Scene graph manager.
	SceneManager graph;

	//Scene renderer.
	SceneRenderer renderer;

public:

	/**
	* Starts the world and begins rendering and updating the scene.
	*/
	virtual void run();
};