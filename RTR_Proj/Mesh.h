#pragma once

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include "Globals.h"
#include "Material.h"

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct BoundingBox
{
	glm::vec3 points[2];
	glm::vec3 center;
};

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
	explicit Mesh(const char *id);
	Mesh(const char *id, SceneManager* manager, vector<Vertex> vertices, vector<GLuint> indices, BoundingBox boundingBox, const char *materialId = nullptr);
	GLuint getVAO();
	virtual void display(glm::mat4 transf, int material, Camera *camera = nullptr, bool shadowMap = false, Globals::LIGHT_TYPE shadowType = Globals::DIRECTIONAL);
	virtual ~Mesh();
	void setupMesh();
	BoundingBox getBoundingBox();
	int getMaterialId();
};

class Model
{
	const char *id;
	vector<Mesh*> meshes;
	SceneManager *manager;
	std::string directory;
public:
	Model(const char *id, SceneManager *manager, const char *path);
	void addMesh(Mesh *mesh);
	const char* getId();
	vector<Mesh*> getMeshes();
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, char *typeName);
	GLint TextureFromFile(const char *path, string directory);
};

