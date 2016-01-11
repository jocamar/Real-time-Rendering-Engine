#pragma once

#include <iostream>
#include "Particle.h"
#include "AttacheableObject.h"

#define MAXPARTICLES 1000

using namespace std;
class Emitter : public AttacheableObject
{
public:
	
	Emitter(const char *id, SceneManager *manager, SceneNode *parent);
	GLfloat* g_particule_position_size_data = new GLfloat[MAXPARTICLES * 4];
	GLubyte* g_particule_color_data = new GLubyte[MAXPARTICLES * 4];
	Particle ParticlesContainer[MAXPARTICLES];
	int LastUsedParticle = 0;
	int ParticlesCount = 0;
	void display(int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override {};
	bool isLeaf() override { return true; }
	void update(float delta) override;
	void Simulate(float delta);
	RenderOrder getRenderEntities(int material, Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;
	int FindUnusedParticle();
};