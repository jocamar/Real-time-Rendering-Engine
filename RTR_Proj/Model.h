#pragma once

#include "Globals.h"
#include "Shader.h"
#include "Mesh.h"
#include <SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Model : public Mesh
{
public:
	/*  Functions   */
	Model(GLchar* path)
	{
		this->loadModel(path);
	}
	void display() override;
	void Draw(Shader shader);
private:
	/*  Model Data  */
	vector<Mesh> meshes;
	vector<Texture> textures_loaded;
	string directory;
	/*  Functions   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		string typeName);
};