#pragma once

#include <iostream>
#include "AttacheableObject.h"

class Emitter;

using namespace std;
class Particle
{
public:
	Particle(Emitter *em = nullptr);
	Emitter *em;
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if < 0 : dead and unused.7
	GLuint billboard_vertex_buffer;
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
	Particle(const char *id, Emitter *em, SceneManager *manager);
	void display(int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL);
	void update();
	~Particle();
};

