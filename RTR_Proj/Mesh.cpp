#include "Mesh.h"
#include "SceneManager.h"
#include "Camera.h"

Mesh::Mesh(const char *id)
{
	this->idMesh = id;
}



Mesh::Mesh(const char *id, SceneManager *manager, vector<Vertex> vertices, vector<GLuint> indices, BoundingBox boundingBox, const char *materialId)
{
	this->idMesh = id;
	this->vertices = vertices;
	this->indices = indices;
	this->manager = manager;
	if(!materialId)
		this->material = -1;
	else this->material = manager->getMaterialNum(materialId);
	this->boundingBox = boundingBox;

	setupMesh();
}



GLuint Mesh::getVAO() {
	return VAO;
}



void Mesh::display(glm::mat4 transf, int material, Camera *camera, bool shadowMap, Globals::LIGHT_TYPE shadowType) {
	int materialToUse;
	if (this->material >= 0)
		materialToUse = this->material;
	else
		materialToUse = material;

	auto s = this->manager->getMaterial(materialToUse);

	s->use(camera, shadowMap, shadowType);

	// Create camera transformation
	glm::mat4 mvm;
	if (!shadowMap)
	{
		glm::mat4 view;
		view = camera->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera->Zoom), (float)1280 / (float)720, 0.1f, 1000.0f);
		mvm = projection * view * transf;
	}
	else
	{
		mvm = camera->ViewProjMatrix * transf;
	}
	// Pass the matrices to the shader
	if(!shadowMap)
	{
		glUniformMatrix4fv(s->getShader()->ModelViewLoc, 1, GL_FALSE, glm::value_ptr(mvm));
		glUniformMatrix4fv(s->getShader()->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));
	}
	else
	{
		if (shadowType == Globals::DIRECTIONAL) {
			glUniformMatrix4fv(manager->getShadowShader()->ModelViewLoc, 1, GL_FALSE, glm::value_ptr(mvm));
		}
		else if(shadowType == Globals::POINT)
		{
			for (GLuint i = 0; i < 6; ++i)
				glUniformMatrix4fv(glGetUniformLocation(manager->getOmniShadowShader()->Program, ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(camera->cubeViewProjectionMatrixes[i]));
			glUniform1f(glGetUniformLocation(manager->getOmniShadowShader()->Program, "far_plane"), 10.0f);
			glUniform3fv(glGetUniformLocation(manager->getOmniShadowShader()->Program, "lightPos"), 1, &(camera->Position[0]));
			glUniformMatrix4fv(manager->getOmniShadowShader()->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));
		}
	}

	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	s->unUse(camera, shadowMap, shadowType);
}



Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}



void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint),
		&this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}



BoundingBox Mesh::getBoundingBox()
{
	return boundingBox;
}



int Mesh::getMaterialId()
{
	return material;
}



Model::Model(const char *id, SceneManager *manager, const char *path)
{
	this->id = id;
	this->manager = manager;

	if(path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace );

		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
			return;
		}
		std::string tmp = path;
		this->directory = tmp.substr(0, tmp.find_last_of('/'));

		this->processNode(scene->mRootNode, scene);
	}
}



void Model::addMesh(Mesh *mesh)
{
	this->meshes.push_back(mesh);
}



const char* Model::getId()
{
	return id;
}



vector<Mesh*> Model::getMeshes()
{
	return meshes;
}



void Model::processNode(aiNode *node, const aiScene *scene)
{
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}



Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture*> textures;

	float max_x = FLT_MIN;
	float max_y = FLT_MIN;
	float max_z = FLT_MIN;

	float min_x = FLT_MAX;
	float min_y = FLT_MAX;
	float min_z = FLT_MAX;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if(vector.x > max_x)
		{
			max_x = vector.x;
		}
		if(vector.x < min_x)
		{
			min_x = vector.x;
		}

		if (vector.y > max_y)
		{
			max_y = vector.y;
		}
		if (vector.y < min_y)
		{
			min_y = vector.y;
		}

		if (vector.z > max_z)
		{
			max_z = vector.z;
		}
		if (vector.z < min_z)
		{
			min_z = vector.z;
		}

		
		// Normals
		if(mesh->mNormals)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		else
		{
			vertex.Normal = glm::vec3(0, 1, 0);
		}

		if (mesh->mTangents)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
		}
		else
		{
			vertex.Tangent = glm::vec3(1, 0, 0);
		}

		if (mesh->mBitangents)
		{
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		else
		{
			vertex.Bitangent = glm::vec3(0, 0, 1);
		}
		
		// Texture Coordinates
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	float mid_x = min_x + ((max_x - min_x) / 2.0f);
	float mid_y = min_y + ((max_y - min_y) / 2.0f);
	float mid_z = min_z + ((max_z - min_z) / 2.0f);
	BoundingBox bBox;
	bBox.points[0] = glm::vec3(max_x, max_y, max_z);
	bBox.points[1] = glm::vec3(min_x, min_y, min_z);
	bBox.center = glm::vec3(mid_x, mid_y, mid_z);

	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	char *materialId = nullptr;
	// Process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// Diffuse: texture_diffuseN
		// Specular: texture_specularN
		// Normal: texture_normalN

		// 1. Diffuse maps
		vector<Texture*> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse_texture");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. Specular maps
		vector<Texture*> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "specular_texture");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		vector<Texture*> bumpMaps = this->loadMaterialTextures(material, aiTextureType_HEIGHT, "normal_texture");
		textures.insert(textures.end(), bumpMaps.begin(), bumpMaps.end());

		std::string tmp = this->id;
		tmp += std::to_string(mesh->mMaterialIndex);
		auto tmp_c = new char[tmp.size() + 1];
		strcpy_s(tmp_c, tmp.size()+1, tmp.c_str());
		tmp_c[tmp.size()] = '\0';
		materialId = tmp_c;

		aiColor3D amb_color(0.f, 0.f, 0.f);
		material->Get(AI_MATKEY_COLOR_AMBIENT, amb_color);
		GLfloat amb[3] = { amb_color.r, amb_color.g, amb_color.b };

		aiColor3D dif_color(0.f, 0.f, 0.f);
		material->Get(AI_MATKEY_COLOR_AMBIENT, dif_color);
		GLfloat dif[3] = { dif_color.r, dif_color.g, dif_color.b };

		aiColor3D spec_color(0.f, 0.f, 0.f);
		material->Get(AI_MATKEY_COLOR_AMBIENT, spec_color);
		GLfloat spec[3] = { spec_color.r, spec_color.g, spec_color.b };

		float shininess = 0.0;
		material->Get(AI_MATKEY_SHININESS, shininess);
		float opacity = 0.0;
		material->Get(AI_MATKEY_OPACITY, opacity);
		int model;
		material->Get(AI_MATKEY_SHADING_MODEL, model);

		manager->addMaterial(tmp_c, "defaultShader.vs", "defaultShader.frag", textures, amb, dif, spec, shininess, opacity, model, Material::LIGHTING_TEXTURED);
	}

	// Return a mesh object created from the extracted mesh data
	return new Mesh(this->id, this->manager, vertices, indices, bBox, materialId);
}



vector<Texture*> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, char *typeName)
{
	vector<Texture*> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		auto t = manager->getTexture(str.C_Str());
		
		if (!t)
		{   // If texture hasn't been loaded already, load it
			Texture *texture = new Texture();
			texture->texture = TextureFromFile(str.C_Str(), this->directory);
			texture->type = typeName;

			string tmp = str.C_Str();
			auto tmp_c = new char[tmp.size() + 1];
			strcpy_s(tmp_c, tmp.size()+1, tmp.c_str());
			tmp_c[tmp.size()] = '\0';
			texture->id = tmp_c;

			textures.push_back(texture);
			this->manager->addTexture(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
		else
		{
			textures.push_back(t);
		}
	}
	return textures;
}



GLint Model::TextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	if(filename[0] != '/')
		filename = directory + '/' + filename;
	else filename = directory + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}