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
public:
	explicit Mesh(const char *id);
	Mesh(const char *id, SceneManager* manager, vector<Vertex> vertices, vector<GLuint> indices, const char *materialId = nullptr);
	GLuint getVAO();
	virtual void display(glm::mat4 transf, int material, Camera *camera = nullptr);
	virtual ~Mesh();
	void setupMesh();
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

