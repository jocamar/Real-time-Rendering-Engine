#pragma once
#include "Globals.h"
#include "Shader.h"

class SceneManager;

class Camera;

class Material
{
protected:
	const char *id;
	Shader *shader;
	GLuint diffuseMap;
	GLuint specularMap;
	GLuint emissionMap;

	int shaderType;

	SceneManager *sceneManager;
public:
	enum shaderTypes {LIGHTING_TEXTURED, LIGHTING, EMITTER};

	Material(const char *id, const char* diffuse, const char* specular, Shader *shader, shaderTypes shaderType, SceneManager *manager);
	~Material();
	Shader* getShader();
	GLuint getDiffuseMap();
	GLuint getSpecMap();
	GLuint getEmissionMap();

	void use(Camera *camera);
};