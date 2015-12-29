#pragma once
#include "Globals.h"
#include "Shader.h"

class SceneManager;

class Camera;

struct Texture {
	const char *id;
	GLuint texture;
	char *type;
};

class Material
{
protected:
	const char *id;
	Shader *shader;

	vector<Texture*> textures;
	GLuint emissionMap;
	GLfloat* ambientIntensity;
	GLfloat* diffuseIntensity;

	int shaderType;

	SceneManager *manager;
public:
	enum shaderTypes {LIGHTING_TEXTURED, LIGHTING, EMITTER};

	Material(const char *id, SceneManager *manager, const char *shaderId , shaderTypes shaderType, GLfloat* ambientI, GLfloat* diffuseI, const char *diffuseId = nullptr, const char *specularId = nullptr);
	Material(const char *id, SceneManager *manager, const char *shaderId, shaderTypes shaderType, GLfloat* ambientI, GLfloat* diffuseI, vector<Texture*> textures);
	~Material();

	const char* getId();
	Shader* getShader();
	vector<Texture*> getDiffuseMaps();
	vector<Texture*> getSpecMaps();
	GLuint getEmissionMap();

	void use(Camera *camera);
};