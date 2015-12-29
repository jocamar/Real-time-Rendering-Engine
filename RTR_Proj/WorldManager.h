#pragma once

#include "Globals.h"
#include "SceneManager.h"
#include "SceneRenderer.h"

class WorldManager
{
	map<string,Entity*> entities;

	SceneManager graph;
	SceneRenderer renderer;

public:
	virtual void run();
};