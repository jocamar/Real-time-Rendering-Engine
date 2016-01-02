#pragma once
#include "SceneNode.h"

#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Mesh.h"

class SceneManager
{
protected:
	SceneNode *root;

	vector<Shader*> shaders;
	vector<Texture*> textures;
	vector<Material*> materials;
	vector<Model*> models;
	int defaultMaterial;

	vector<Light*> lights;
	Light *directionalLight;
	Shader *shadowShader;

public:

	SceneManager();

	void addMaterial(const char *id, const char *vert, const char *frag, char *diffuse = nullptr, char *specular = nullptr, char *normal = nullptr, GLfloat *ambientI = nullptr, 
												GLfloat *diffuseI = nullptr, GLfloat *specularI = nullptr, GLfloat shininess = 32.0, GLfloat opacity = 1.0, 
												int shadingModel = 1, Material::shaderTypes shaderType = Material::LIGHTING, GLint interpMethod = GL_REPEAT);

	void addMaterial(const char *id, const char *vert, const char *frag, vector<Texture*> textures, GLfloat *ambientI = nullptr, GLfloat *diffuseI = nullptr,
												GLfloat *specularI = nullptr, GLfloat shininess = 32.0, GLfloat opacity = 1.0, int shadingModel = 1,
												Material::shaderTypes shaderType = Material::LIGHTING);
	void setDefaultMaterial(int num);
	Material* getDefaultMaterial();
	Material* getMaterial(int material);
	Material* getMaterial(const char *id);
	int getMaterialNum(const char *id);
	int getMaterialNum(Material *material);

	Shader* getShader(int shader);
	Shader* getShader(const char *id);
	int getShaderNum(const char *id);
	int getShaderNum(Shader *shader);

	void addTexture(Texture *texture);
	Texture* getTexture(int texture);
	Texture* getTexture(const char *id);
	int getTextureNum(const char *id);
	int getTextureNum(Texture *texture);

	void addModel(const char *id, const char *model);
	Model* getModel(int model);
	Model* getModel(const char *id);
	int getModelNum(const char *id);
	int getModelNum(Model *model);

	Entity* createEntity(const char *id, const char *modelId);

	Light* createLight(const char *id, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, const char *modelId);

	void createDirectionalLight(const char *id, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat *direction);
	vector<Light*> getActiveLights();
	Light* getDirectionalLight();

	SceneNode* getRoot();
	
	void update(float millis);
	void render(Camera *camera, bool shadowMap = false);
	void generateShadowMaps();

	void getRenderNodes();
	Shader* getShadowShader();
};