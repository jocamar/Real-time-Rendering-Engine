#pragma once
#include "Globals.h"
#include "Shader.h"

class SceneManager;

class Camera;

/**
* Struct representing a texture in memory.
*/
struct Texture {
	//Unique texture ID.
	const char *id;

	//Texture OpenGL id.
	GLuint texture;

	//Texture type (diffuse_texture, specular_texture, normal_texture).
	char *type;
};

/**
* Class representing a material to be used by a node, mesh or entity in the scene.
*/
class Material
{
protected:
	//Unique ID for this material.
	const char *id;

	//The shader to be applied by this material.
	Shader *shader;

	//Textures used by this material.
	vector<Texture*> textures;

	//Cubemap texture used by this material.
	//@TODO this was supposed to be used for reflections, check if it makes sense to keep it or if there is a better way to do this.
	GLuint cubemapTexture;

	//Emission map for this material.
	//@TODO check what this does and if it makes sense to keep it.
	GLuint emissionMap;

	//Ambient intensity of this material.
	GLfloat* ambientIntensity;

	//Diffuse intensity of this material.
	GLfloat* diffuseIntensity;

	//Specular intensity of this material.
	GLfloat* specularIntensity;

	//Shininess of this material.
	GLfloat shininess;

	//Opacity of this material.
	GLfloat opacity;

	//Shading model used by this material (0 - no shading, 1 - diffuse shading, 2 - diffuse and specular).
	int shadingModel;

	//Specifies whether this material contains transparency.
	bool transparent;


	SceneManager *manager;
public:
	//Type of the shader used by this material (indicates what kind of parameters must be set in the shader).
	int shaderType;

	enum shaderTypes {LIGHTING_TEXTURED, LIGHTING, EMITTER, REFLECTIVE, PARTICLE, TEXTURED};

	/**
	* Material constructor.
	*
	* @param id is the unique ID for this material.
	* @param manager is the scene manager to which this material is assigned.
	* @param shaderId is the ID of the shader that this material uses.
	* @param shaderType is the type of the shader used by this material.
	* @param ambientI is the ambient intensity of this material.
	* @param diffuseI is the diffuse intensity of this material.
	* @param specularI is the specular intensity of this material.
	* @param shininess is the shininess of this material.
	* @param opacity is the opacity of this material.
	* @param shadingModel is the shadingModel used by this material (0 - no shading, 1 - diffuse shading, 2 - diffuse and specular).
	* @param diffuseId is the diffuse texture Id to be used by this material.
	* @param specularId is the specular texture Id to be used by this material.
	* @param normalId is the bump texture Id to be used by this material.
	*/
	Material(const char *id, SceneManager *manager, const char *shaderId , shaderTypes shaderType, GLfloat *ambientI, GLfloat *diffuseI, GLfloat *specularI, GLfloat shininess, 
					GLfloat opacity, int shadingModel, const char *diffuseId = nullptr, const char *specularId = nullptr, const char *normalId = nullptr);

	/**
	* Material constructor.
	*
	* @param id is the unique ID for this material.
	* @param manager is the scene manager to which this material is assigned.
	* @param shaderId is the ID of the shader that this material uses.
	* @param shaderType is the type of the shader used by this material.
	* @param ambientI is the ambient intensity of this material.
	* @param diffuseI is the diffuse intensity of this material.
	* @param specularI is the specular intensity of this material.
	* @param shininess is the shininess of this material.
	* @param opacity is the opacity of this material.
	* @param shadingModel is the shadingModel used by this material (0 - no shading, 1 - diffuse shading, 2 - diffuse and specular).
	* @param textures is a vector containing the textures used by this material.
	*/
	Material(const char *id, SceneManager *manager, const char *shaderId, shaderTypes shaderType, GLfloat *ambientI, GLfloat *diffuseI, GLfloat *specularI, GLfloat shininess,
					GLfloat opacity, int shadingModel, vector<Texture*> textures);

	/**
	* Material constructor.
	* @TODO check if this is needed, while this was supposed to be used for reflections, where the material 
	*		had to have a cubemap texture, there may be a better way to handle it and it may not even be behaving as expected.
	*
	* @param id is the unique ID for this material.
	* @param manager is the scene manager to which this material is assigned.
	* @param shaderId is the ID of the shader that this material uses.
	* @param shaderType is the type of the shader used by this material.
	* @param ambientI is the ambient intensity of this material.
	* @param diffuseI is the diffuse intensity of this material.
	* @param specularI is the specular intensity of this material.
	* @param shininess is the shininess of this material.
	* @param opacity is the opacity of this material.
	* @param shadingModel is the shadingModel used by this material (0 - no shading, 1 - diffuse shading, 2 - diffuse and specular).
	* @param faces is a vector containing the cubemap faces used by this material.
	*/
	Material(const char *id, SceneManager *manager, const char *shaderId, shaderTypes shaderType, GLfloat *ambientI, GLfloat *diffuseI, GLfloat *specularI, GLfloat shininess,
		GLfloat opacity, int shadingModel, vector<const GLchar*> faces);

	/**
	* Material destructor.
	*/
	~Material();

	/**
	* Returns this material's unique ID.
	*/
	const char* getId();

	/**
	* Returns the shader used by this material.
	*/
	Shader* getShader();

	/**
	* Returns the diffuse maps used by this material.
	*/
	vector<Texture*> getDiffuseMaps();

	/**
	* Returns the specular maps used by this material.
	*/
	vector<Texture*> getSpecMaps();

	/**
	* Returns the emission map used by this material.
	*/
	GLuint getEmissionMap();

	/**
	* Returns whether this material has transparency.
	*/
	bool isTransparent();

	/**
	* Loads the specified cubemap faces into a cubemap for this material.
	* @TODO check if this is needed, while this was supposed to be used for reflections, where the material
	*		had to have a cubemap texture, there may be a better way to handle it and it may not even be behaving as expected.
	*/
	GLuint loadCubemap(vector<const GLchar*> faces);

	/**
	* Applies this material, setting the shader parameters correctly (Note, it does not apply the shader for performance reasons in order to minimise gl calls, that is done in th erender function).
	* @TODO check if the shadow map information is needed, can probably be removed and make the check on the render function, not using the material if shadowMap is true.
	*
	* @param camera is the camera being used to render the scene.
	* @param shadowMap specifies whether the material is being applied when rendering the shadowMaps.
	* @param shadowType specifies the type of shadowMaps being rendered.
	*/
	void use(Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL);

	/**
	* Un-applies this material, setting the shader parameters back to the defaults (needed because textures that are not used by a shader need to be set to default black).
	* @TODO check if the shadow map information is needed, can probably be removed and make the check on the render function, not using the material if shadowMap is true.
	*
	* @param camera is the camera being used to render the scene.
	* @param shadowMap specifies whether the material is being applied when rendering the shadowMaps.
	* @param shadowType specifies the type of shadowMaps being rendered.
	*/
	void unUse(Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL);
};