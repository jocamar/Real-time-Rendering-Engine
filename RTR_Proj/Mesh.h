#pragma once

#include "Globals.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;  // We store the path of the texture to compare with other textures
};

class Mesh
{
protected:
	/*  Render data  */
	GLuint VAO, VBO, EBO;
	GLuint *buffers;
	GLuint numBuffers;
	string idMesh;
private:
	/*  Functions    */
	void setupMesh();
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	/*  Functions  */
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	Mesh() {};
	void Draw(Shader shader);


	GLuint getVAO();
	virtual void display();
	virtual ~Mesh();

};

