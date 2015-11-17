#pragma once
#include "Globals.h"
#include "Shader.h"

class Material
{
protected:
	const char *id;
	Shader *shader;
	GLuint diffuseMap;
	GLuint specularMap;
	GLuint emissionMap;
public:
	Material(const char *id, const char* diffuse, const char* specular, Shader *shader);
};