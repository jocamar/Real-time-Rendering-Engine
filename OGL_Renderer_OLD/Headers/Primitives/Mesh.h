#pragma once

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "Globals.h"
#include "Material.h"

using namespace std;

/**
* Struct representing a vertex, with its position, normal, uv coordinates, tangent and bitangent.
*/
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

/**
* Struct representing an object's bounding box.
*/
struct BoundingBox
{
	glm::vec3 points[2];
	glm::vec3 center;
};

/**
* Class representing an arbitrary mesh of vertexes on the scene. Base class to other primitives.
*/
class Mesh
{
protected:
	GLuint VAO, VBO, EBO;
	const char *idMesh;
	
	SceneManager *manager;
	vector<Vertex> vertices;
	vector<GLuint> indices;
	int material;
	BoundingBox boundingBox;
public:

	/**
	* Mesh constructor.
	*
	* @param id is a unique primitive ID used to retrieve the object.
	*/
	explicit Mesh(const char *id);

	/**
	* Mesh constructor.
	*
	* @param id is a unique primitive ID used to retrieve the object.
	* @param manager is the scene manager to which this object is assigned.
	* @param vertices is the vector containing the vertexes for the mesh.
	* @param indices is the vector containing the indices for the vertexes to be used for the mesh.
	* @param boundingBox is the mesh's bounding box.
	* @param materialId is the unique ID for the material to use for this object.
	*/
	Mesh(const char *id, SceneManager* manager, vector<Vertex> vertices, vector<GLuint> indices, BoundingBox boundingBox, const char *materialId = nullptr);

	/**
	* Returns the object's Vertex Array Object.
	*/
	GLuint getVAO();

	/**
	* Displays the mesh.
	*
	* @param transf is the object's model matrix.
	* @param material is the object's material id number.
	* @param camera is the current camera used to render.
	* @param shadowMap is a boolean indicating whether the object is being rendered for generating the shadow maps.
	* @param shadowType indicates what type of shadow maps are being generated, directional or omni-directional.
	*/
	virtual void display(glm::mat4 transf, int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL);

	/**
	* Mesh destructor.
	*/
	virtual ~Mesh();

	/**
	* Generates and binds the necessary buffers for the object from provided vertex and index vectors.
	* @TODO check if it can be made private.
	*/
	void setupMesh();

	/**
	* Returns the object's bounding box.
	*/
	BoundingBox getBoundingBox();

	/**
	* Returns the object's material id.
	*/
	int getMaterialId();
};


/**
* Class representing a model on the scene. A model can be composed of several meshes with several materials.
*/
class Model
{
	const char *id;
	vector<Mesh*> meshes;
	SceneManager *manager;
	std::string directory;
public:

	/**
	* Model constructor.
	*
	* @param id is a unique ID used to retrieve the object.
	* @param manager is the scene manager to which this object is assigned.
	* @param path is the path to the directory to load the model from.
	*/
	Model(const char *id, SceneManager *manager, const char *path);

	/**
	* Adds a mesh to this model.
	*
	* @param mesh is the mesh to add to the model.
	*/
	void addMesh(Mesh *mesh);

	/**
	* Returns the model's ID.
	*/
	const char* getId();

	/**
	* Returns the model's meshes.
	*/
	vector<Mesh*> getMeshes();

	/**
	* Processes an assimp node and all its children, retrieving the meshes.
	* @TODO check if it can be made private.
	*
	* @param node is the assimp node to be processed.
	* @param scene is the assimp scene being processed.
	*/
	void processNode(aiNode* node, const aiScene* scene);

	/**
	* Processes an assimp mesh, retrieving all the necessary vertex and material information.
	* @TODO check if it can be made private.
	*
	* @param mesh is the assimp mesh to be processed.
	* @param scene is the assimp scene being processed.
	*/
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	/**
	* Loads the required textures for an assimp material.
	* @TODO check if it can be made private.
	*
	* @param mat is the assimp material to load the textures.
	* @param type is the type of the texture being loaded (diffuse, specular, height).
	* @param typename is the type name of the texture (diffuse_texture, specular_texture, normal_texture).
	*/
	vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, char *typeName);

	/**
	* Loads a texture into memory, generating and binding it to a texture ID.
	* @TODO check if it can be made private.
	*
	* @param path is the path to the texture.
	* @param directory is the directory where the texture is located.
	*/
	GLint TextureFromFile(const char *path, string directory);
};

