#pragma once
#include "SceneNode.h"

#include "Material.h"
#include "Entity.h"
#include "Light.h"
#include "Primitives/Mesh.h"

/**
* Struct representing a render order to use during rendering.
* Contains an ordered vector of sub-entities.
*/
struct RenderOrder
{
	vector<SubEntity*> Entities;
};

/**
* Class representing a scene manager, that manages the scene graph, its nodes, materials, lights and entities.
*/
class SceneManager
{
protected:
	//The root node of the graph.
	SceneNode *root;

	vector<Shader*> shaders;
	vector<Texture*> textures;
	vector<Material*> materials;
	vector<Model*> models;

	//Default materia id to be used when no other material is specified.
	int defaultMaterial;

	//Current material being used during rendering.
	int currentMaterial;

	//Lights in the scene. A maximum of 8 lights can be rendered.
	vector<Light*> lights;

	//Directional light in the scene.
	Light *directionalLight;

	//Shaders for the shadows.
	Shader *shadowShader;
	Shader *omniShadowShader;

	//Current shader being used during rendering.
	Shader *currentShader;

	//Render orders to be used during the different rendering steps.
	RenderOrder directionalShadowOrder;
	RenderOrder pointShadowOrder;
	RenderOrder regularOrder;

	//Booleans determining whether the rendering orders need to be updated.
	bool orderUpdatedDirectional;
	bool orderUpdatedPoint;
	bool orderUpdatedRegular;

public:
	/**
	* Scene manager constructor.
	*/
	SceneManager();

	//----------------------------------------------------------------------------------------------------------------------
	//                                                  MATERIALS
	//----------------------------------------------------------------------------------------------------------------------

	/**
	* Adds a new material to the manager, registering it for use by the entities and nodes in the graph.
	* @TODO probably clean this up, too many parameters.
	*
	* @param id is the unique ID of the material to add.
	* @param vert is the path to the vertex shader of the material.
	* @param frag is the path to the fragment shader of the material.
	* @param diffuse is the path to the diffuse texture of the material.
	* @param specular is the path to the specular texture of the material.
	* @param normal is the path to the bump texture of the material.
	* @param ambientI is the ambient light intensities of this material.
	* @param diffuseI is the diffuse light intensities of this material.
	* @param specularI is the specular light intensities of this material.
	* @param shininess is the shininess of this material.
	* @param opacity is the opacity of this material.
	* @param shadingModel is the shading model of this material (0 - no shading, 1 - diffuse shading, 2 - diffuse and specular).
	* @param shaderType is the type of the shader used by this material (the type of shader determines which parameters must be set in the shader).
	* @param interpMethod is the interpolation method to be used with the textures of the material.
	*/
	void addMaterial(const char *id, const char *vert, const char *frag, char *diffuse = nullptr, char *specular = nullptr, char *normal = nullptr, GLfloat *ambientI = nullptr, 
												GLfloat *diffuseI = nullptr, GLfloat *specularI = nullptr, GLfloat shininess = 32.0, GLfloat opacity = 1.0, 
												int shadingModel = 1, Material::shaderTypes shaderType = Material::LIGHTING, GLint interpMethod = GL_REPEAT);

	/**
	* Adds a new material to the manager, registering it for use by the entities and nodes in the graph.
	* @TODO probably clean this up, too many parameters.
	*
	* @param id is the unique ID of the material to add.
	* @param vert is the path to the vertex shader of the material.
	* @param frag is the path to the fragment shader of the material.
	* @param textures is a vector containing the textures to be used by this material.
	* @param ambientI is the ambient light intensities of this material.
	* @param diffuseI is the diffuse light intensities of this material.
	* @param specularI is the specular light intensities of this material.
	* @param shininess is the shininess of this material.
	* @param opacity is the opacity of this material.
	* @param shadingModel is the shading model of this material (0 - no shading, 1 - diffuse shading, 2 - diffuse and specular).
	* @param shaderType is the type of the shader used by this material (the type of shader determines which parameters must be set in the shader).
	*/
	void addMaterial(const char *id, const char *vert, const char *frag, vector<Texture*> textures, GLfloat *ambientI = nullptr, GLfloat *diffuseI = nullptr,
												GLfloat *specularI = nullptr, GLfloat shininess = 32.0, GLfloat opacity = 1.0, int shadingModel = 1,
												Material::shaderTypes shaderType = Material::LIGHTING);

	/**
	* Adds a new material to the manager, registering it for use by the entities and nodes in the graph.
	* @TODO probably clean this up, too many parameters.
	* @TODO check if this is needed, while this was supposed to be used for reflections, where the material 
	*		had to have a cubemap texture, there may be a better way to handle it and it may not even be behaving as expected.
	*
	* @param id is the unique ID of the material to add.
	* @param vert is the path to the vertex shader of the material.
	* @param frag is the path to the fragment shader of the material.
	* @param faces are the cubemap faces for the cubemap textures.
	* @param ambientI is the ambient light intensities of this material.
	* @param diffuseI is the diffuse light intensities of this material.
	* @param specularI is the specular light intensities of this material.
	* @param shininess is the shininess of this material.
	* @param opacity is the opacity of this material.
	* @param shadingModel is the shading model of this material (0 - no shading, 1 - diffuse shading, 2 - diffuse and specular).
	* @param shaderType is the type of the shader used by this material (the type of shader determines which parameters must be set in the shader).
	*/
	void addMaterial(const char *id, const char *vert, const char *frag, vector<const GLchar*> faces, GLfloat *ambientI = nullptr, GLfloat *diffuseI = nullptr,
												GLfloat *specularI = nullptr, GLfloat shininess = 32.0, GLfloat opacity = 1.0, int shadingModel = 1,
												Material::shaderTypes shaderType = Material::LIGHTING);

	/**
	* Sets the default material to be used when no other material is provided.
	*
	* @param num is the numeric id of the material to use.
	*/
	void setDefaultMaterial(int num);

	/**
	* Returns the default material to be used when no other material is provided.
	*/
	Material* getDefaultMaterial();

	/**
	* Returns the material specified by the provided numeric ID.
	*
	* @param material is the numeric ID of the material to retrieve.
	*/
	Material* getMaterial(int material);

	/**
	* Returns the material specified by the provided ID.
	*
	* @param id is the ID of the material to retrieve.
	*/
	Material* getMaterial(const char *id);

	/**
	* Returns the numeric ID of the material with the specified ID.
	*
	* @param id is the ID of the material to retrieve.
	*/
	int getMaterialNum(const char *id);

	/**
	* Returns the numeric ID of the specified material.
	*
	* @param material is the material to retrieve.
	*/
	int getMaterialNum(Material *material);

	//----------------------------------------------------------------------------------------------------------------------
	//                                                  SHADERS
	//----------------------------------------------------------------------------------------------------------------------

	/**
	* Returns the shader specified by the given numeric ID.
	*
	* @param shader is the numeric ID of the shader to retrieve.
	*/
	Shader* getShader(int shader);

	/**
	* Returns the shader specified by the given ID.
	*
	* @param id is the ID of the shader to retrieve.
	*/
	Shader* getShader(const char *id);

	/**
	* Returns the numeric shader ID of the shader specified by the given ID.
	*
	* @param id is the ID of the shader to retrieve.
	*/
	int getShaderNum(const char *id);

	/**
	* Returns the numeric shader ID of the specified shader.
	*
	* @param shader is the ID of the shader to retrieve.
	*/
	int getShaderNum(Shader *shader);

	/**
	* Returns the shader used to calculate the directional shadow maps.
	*/
	Shader* getShadowShader();

	/**
	* Returns the shader used to calculate the omni-directional shadow maps.
	*/
	Shader* getOmniShadowShader();

	//----------------------------------------------------------------------------------------------------------------------
	//                                                  TEXTURES
	//----------------------------------------------------------------------------------------------------------------------

	/**
	* Adds the specified texture to the scene manager, registering it so that it can be used by materials.
	*
	* @param texture is the texture to be added.
	*/
	void addTexture(Texture *texture);

	/**
	* Returns the texture specified by the given numeric ID.
	*
	* @param texture is the numeric ID of the texture to retrieve.
	*/
	Texture* getTexture(int texture);

	/**
	* Returns the texture specified by the given ID.
	*
	* @param id is the ID of the texture to retrieve.
	*/
	Texture* getTexture(const char *id);

	/**
	* Returns the numeric texture ID of the texture specified by the given ID.
	*
	* @param id is the ID of the texture to retrieve.
	*/
	int getTextureNum(const char *id);

	/**
	* Returns the numeric texture ID of the specified texture.
	*
	* @param texture is the texture to retrieve.
	*/
	int getTextureNum(Texture *texture);

	//----------------------------------------------------------------------------------------------------------------------
	//                                                  MODELS
	//----------------------------------------------------------------------------------------------------------------------

	/**
	* Adds the specified model to the scene manager, registering it so that it can be used by entities in the graph.
	*
	* @param id is the unique id of the model to be registered.
	* @param model is the path to the model file to be loaded.
	*/
	void addModel(const char *id, const char *model);

	/**
	* Returns the model specified by the given numeric ID.
	*
	* @param model is the numeric id of the model to retrieve.
	*/
	Model* getModel(int model);

	/**
	* Returns the model specified by the given ID.
	*
	* @param id is the id of the model to retrieve.
	*/
	Model* getModel(const char *id);

	/**
	* Returns the numeric ID of the model specified by the given ID.
	*
	* @param id is the id of the model to retrieve.
	*/
	int getModelNum(const char *id);

	/**
	* Returns the numeric ID of the specified model.
	*
	* @param model is the model to retrieve.
	*/
	int getModelNum(Model *model);

	//----------------------------------------------------------------------------------------------------------------------
	//                                                  ENTITIES
	//----------------------------------------------------------------------------------------------------------------------

	/**
	* Creates and registers an entity in the scene manager and returns the entity. This entity must then be attached to a node in the graph to be rendered.
	*
	* @param id is the unique id of the entity to be created.
	* @param modelId is the ID of the model to be assigned to this entity.
	* @param shadowCaster specifies whether this entity casts shadows or not.
	* @param transparent specifies whether this entity has transparency or not.
	*/
	Entity* createEntity(const char *id, const char *modelId, bool shadowCaster = true, bool transparent = false);

	//----------------------------------------------------------------------------------------------------------------------
	//                                                  LIGHTS
	//----------------------------------------------------------------------------------------------------------------------

	/**
	* Creates and registers an omni-directional light in the scene manager and returns the light. This light must then be attached to a node in the graph to have an effect on the scene.
	*
	* @param id is the unique id of the light to be created.
	* @param ambient is the ambient intensities of this light.
	* @param diffuse is the diffuse intensities of this light.
	* @param specular is the specular intensities of this light.
	* @param constant is the constant attenuation factor of this light.
	* @param linear is the linear attenuation factor of this light.
	* @param quadratic is the quadratic attenuation factor of this light.
	* @param modelId is the optional model to be used to represent this light in the scene.
	*/
	Light* createLight(const char *id, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, const char *modelId);

	/**
	* Creates and registers a directional light in the scene manager.
	*
	* @param id is the unique id of the light to be created.
	* @param ambient is the ambient intensities of this light.
	* @param diffuse is the diffuse intensities of this light.
	* @param specular is the specular intensities of this light.
	* @param direction is the direction of the light.
	*/
	void createDirectionalLight(const char *id, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat *direction);

	/**
	* Returns the active lights in the scene.
	*/
	vector<Light*> getActiveLights();

	/**
	* Returns the directional light in the scene.
	*/
	Light* getDirectionalLight();

	/**
	* Generates the shadow maps for the lights in the scene, when the scene is being rendered with the specified camera.
	*
	* @param camera represents the camera rendering the scene. (Currently this parameter is only used to make sure the directional light frustum follows the camera)
	*/
	void generateShadowMaps(Camera *camera);

	//----------------------------------------------------------------------------------------------------------------------
	//                                                  RENDERING
	//----------------------------------------------------------------------------------------------------------------------

	/**
	* Returns the root node of the scene graph.
	*/
	SceneNode* getRoot();

	/**
	* Updates the scene graph every frame, updating the animations, transforms and materials of the nodes and entities in the graph.
	*
	* @param millis is the number of milliseconds elapsed since the last update.
	*/
	void update(float millis);

	/**
	* Renders the scene from the view of the specified camera. 
	* First it retrieves the entities to render and orders them according to their render order and then it draws them in order.
	*
	* @param camera is the camera being used to view the scene.
	* @param shadowMap indicates whether the scene is being rendered to generate the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated.
	*/
	void render(Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL);

	/**
	* Goes throgh the scene graph, culling entities that are not to be rendered and returning the entities to be rendered in order.
	*
	* @param camera is the camera being used to view the scene.
	* @param shadowMap indicates whether the scene is being rendered to generate the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated.
	*/
	RenderOrder getRenderEntities(Camera *camera, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL);
};