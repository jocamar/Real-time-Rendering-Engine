#pragma once

#include "Scene Graph/AttacheableObject.h"
#include "Scene Graph/Entity.h"

class Mesh;

const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
const GLuint DIR_SHADOW_WIDTH = 1920, DIR_SHADOW_HEIGHT = 1920;

using namespace std;

/**
* Class representing a light in the scene graph.
*/
class Light : public Entity
{
	//Ambient intensity.
	GLfloat *ambient;

	//Diffuse intensity.
	GLfloat *diffuse;

	//Specular intensity.
	GLfloat *specular;

	//Attenuation factors.
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

	//Direction for directional lights.
	GLfloat *direction;

	//Specifies whether the light is directional or not.
	bool directional;

	//Framebuffer object to attach the depth maps to in order to update them.
	GLuint depthMapFBO;

	//Textures to hold the depth information.
	GLuint depthMap;
	GLuint depthCubemap;

	//Camera from the light's point of view to be used when rendering the scene to generate the shadow maps.
	Camera *cam;

	//Light space matrix to be passed to the shaders for shadow calculations
	glm::mat4 lightSpaceMatrix;

	//View projetion matrixes for the six sides of the cube for omni-directional lights.
	std::vector<glm::mat4> cubeLightSpaceMatrixes;

public:

	/**
	* Omni-directional light constructor.
	*
	* @param idEntity is the unique id of the light.
	* @param manager is the scene manager to which this light is assigned.
	* @param ambient is the ambient intensity of this light.
	* @param diffuse is the diffuse intensity of this light.
	* @param specular is the specular intensity of this light.
	* @param constant is the constant attenuation factor of this light.
	* @param linear is the linear attenuation factor of this light.
	* @param quadratic is the quadratic attenuation factor of this light.
	* @param modelId is the id of the optional model to represent this light in the scene.
	* @param parent is the parent node if this light.
	*/
	Light(const char *idEntity, SceneManager *manager, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, const char *modelId, SceneNode *parent = nullptr);
	
	/**
	* Directional light constructor.
	*
	* @param idEntity is the unique id of the light.
	* @param manager is the scene manager to which this light is assigned.
	* @param ambient is the ambient intensity of this light.
	* @param diffuse is the diffuse intensity of this light.
	* @param specular is the specular intensity of this light.
	* @param direction is the direction of the directional light.
	*/
	Light(const char *idEntity, SceneManager *manager, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat *direction);

	/**
	* Displays this light if it has a model.
	* @TODO check whether this function is being used at all, delete if not.
	*
	* @param material is the object's material id number.
	* @param camera is the current camera used to render.
	* @param shadowMap is a boolean indicating whether the object is being rendered for generating the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated, directional or omni-directional.
	*/
	void display(int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL) override;

	/**
	* Returns whether this object is a leaf in the graph.
	*/
	bool isLeaf() override;

	/**
	* Updates the object on each cycle. Currently does nothing.
	*
	* @param seconds represents the elapsed seconds since the last update.
	* @TODO possibly change this to a long representing milliseconds.
	*/
	void update(float seconds) override {};

	/**
	* Generates the shadow map for this light when the scene is being rendered from the specified camera's point of view.
	*
	* @param camera is the camera from which the scene is being viewed.
	*/
	void generateShadowMap(Camera *camera);

	/**
	* Returns whether this is a directional light or not.
	*/
	bool isDirectional();

	/**
	* Returns the ambient intensity of this light.
	*/
	GLfloat* getAmbient();

	/**
	* Returns the diffuse intensity of this light.
	*/
	GLfloat* getDiffuse();

	/**
	* Returns the specular intensity of this light.
	*/
	GLfloat* getSpecular();

	/**
	* Returns the direction of this light if it is a directional light.
	*/
	GLfloat* getDirection();

	/**
	* Returns the light space matrix of this light.
	*/
	glm::mat4* getLightSpaceMatrix();

	/**
	* Returns the shadow map for this light if it is a directional light.
	*/
	GLuint getShadowMap();

	/**
	* Returns the shadow cube map for this light if it is an omni-directional light.
	*/
	GLuint getCubeShadowMap();

	/**
	* Returns the constant attenuation factor for this light if it is an omni-directional light.
	*/
	GLfloat getConstant();

	/**
	* Returns the linear attenuation factor for this light if it is an omni-directional light.
	*/
	GLfloat getLinear();

	/**
	* Returns the quadratic attenuation factor for this light if it is an omni-directional light.
	*/
	GLfloat getQuadratic();
};