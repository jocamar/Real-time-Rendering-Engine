#pragma once

#include <iostream>
#include "Particle.h"
#include "AttacheableObject.h"

using namespace std;
class Emitter : public AttacheableObject
{
public:
	GLfloat* g_particule_position_size_data = new GLfloat[100000 * 4];
	GLubyte* g_particule_color_data = new GLubyte[100000 * 4];
	Particle ParticlesContainer[100000];
	int LastUsedParticle = 0;
	int ParticlesCount = 0;
	void update(float delta);
	void Simulate(float delta);
	int FindUnusedParticle();
};