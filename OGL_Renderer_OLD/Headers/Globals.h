#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SOIL.h>

#include <vector>
using std::vector;

//Defines the maximum number of lights that can be in the scene.
#define MAX_LIGHTS 9

class Globals
{
public:
	enum LIGHT_TYPE { DIRECTIONAL, POINT, SPOT };
	vector<float> background;
	const char *drawmode;
	const char *shading;
	const char *cullface;
	const char *cullorder;
	Globals(vector<float> background,const char *drawmode,const char *shading,const char *cullface,const char *cullorder);
	~Globals(void);
};
