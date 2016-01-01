#include "SceneManager.h"

SceneManager::SceneManager()
{
	this->root = new SceneNode("_root", this);
	this->materials = vector<Material*>();
}



void SceneManager::addMaterial(const char *id, const char *vert, const char *frag, char *diffuse, char *specular, char *normal, GLfloat *ambientI,
										GLfloat *diffuseI, GLfloat *specularI, GLfloat shininess, GLfloat opacity, int shadingModel,
										Material::shaderTypes shaderType, GLint interpMethod)
{
	Material *m = this->getMaterial(id);

	if (m) return;

	GLfloat* tmpAmbient = new GLfloat[3];
	GLfloat* tmpDiffuse = new GLfloat[3];
	GLfloat* tmpSpecular = new GLfloat[3];

	string shaderId = vert;
	shaderId += frag;

	Shader *shader = nullptr;
	for(auto s : shaders)
	{
		if(strcmp(s->id,shaderId.c_str()) == 0)
		{
			shader = s;
			break;
		}
	}

	if(!shader)
	{
		shader = new Shader(vert, frag);
		shaders.push_back(shader);
	}

	Texture *texture = nullptr;
	int width, height;
	unsigned char* image;

	if(diffuse)
	{
		for (auto t : textures)
		{
			if (strcmp(t->id, diffuse) == 0)
			{
				texture = t;
				break;
			}
		}

		if (!texture)
		{
			texture = new Texture();
			texture->id = diffuse;
			glGenTextures(1, &texture->texture);
			texture->type = "diffuse_texture";
			// Diffuse map
			image = SOIL_load_image(diffuse, &width, &height, 0, SOIL_LOAD_RGBA);
			
			glBindTexture(GL_TEXTURE_2D, texture->texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, interpMethod);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, interpMethod);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			textures.push_back(texture);
		}
		texture = nullptr;
	}

	if (specular)
	{
		for (auto t : textures)
		{
			if (strcmp(t->id, specular) == 0)
			{
				texture = t;
				break;
			}
		}

		if (!texture)
		{
			texture = new Texture();
			texture->id = specular;
			glGenTextures(1, &texture->texture);
			texture->type = "specular_texture";
			// Specular map
			image = SOIL_load_image(specular, &width, &height, 0, SOIL_LOAD_RGBA);
			glBindTexture(GL_TEXTURE_2D, texture->texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, interpMethod);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, interpMethod);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			textures.push_back(texture);
		}
		texture = nullptr;
	}

	if (normal)
	{
		for (auto t : textures)
		{
			if (strcmp(t->id, normal) == 0)
			{
				texture = t;
				break;
			}
		}

		if (!texture)
		{
			texture = new Texture();
			texture->id = normal;
			glGenTextures(1, &texture->texture);
			texture->type = "normal_texture";
			// Specular map
			image = SOIL_load_image(normal, &width, &height, 0, SOIL_LOAD_RGBA);
			glBindTexture(GL_TEXTURE_2D, texture->texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			SOIL_free_image_data(image);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, interpMethod);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, interpMethod);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			textures.push_back(texture);
		}
		texture = nullptr;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	
		 
	if (!ambientI)
	{
		tmpAmbient[0] = 1.0f;
		tmpAmbient[1] = 1.0f;
		tmpAmbient[2] = 1.0f;
	}
	else
	{
		tmpAmbient[0] = ambientI[0];
		tmpAmbient[1] = ambientI[1];
		tmpAmbient[2] = ambientI[2];
	}

	if (!diffuseI)
	{
		tmpDiffuse[0] = 1.0f;
		tmpDiffuse[1] = 1.0f;
		tmpDiffuse[2] = 1.0f;
	}
	else
	{
		tmpDiffuse[0] = diffuseI[0];
		tmpDiffuse[1] = diffuseI[1];
		tmpDiffuse[2] = diffuseI[2];
	}

	if (!specularI)
	{
		tmpSpecular[0] = 1.0f;
		tmpSpecular[1] = 1.0f;
		tmpSpecular[2] = 1.0f;
	}
	else
	{
		tmpSpecular[0] = specularI[0];
		tmpSpecular[1] = specularI[1];
		tmpSpecular[2] = specularI[2];
	}

	m = new Material(id, this, shaderId.c_str(), shaderType, tmpAmbient, tmpDiffuse, tmpSpecular, shininess, opacity, shadingModel, diffuse, specular, normal);
	this->materials.push_back(m);
}



void SceneManager::addMaterial(const char *id, const char *vert, const char *frag, vector<Texture*> textures, GLfloat *ambientI, GLfloat *diffuseI, 
									GLfloat *specularI, GLfloat shininess, GLfloat opacity, int shadingModel, Material::shaderTypes shaderType)
{
	Material *m = this->getMaterial(id);

	if (m) return;

	GLfloat* tmpAmbient = new GLfloat[3];
	GLfloat* tmpDiffuse = new GLfloat[3];
	GLfloat* tmpSpecular = new GLfloat[3];

	string shaderId = vert;
	shaderId += frag;

	Shader *shader = nullptr;
	for (auto s : shaders)
	{
		if (strcmp(s->id, shaderId.c_str()) == 0)
		{
			shader = s;
			break;
		}
	}

	if (!shader)
	{
		shader = new Shader(vert, frag);
		shaders.push_back(shader);
	}


	if (!ambientI)
	{
		tmpAmbient[0] = 1.0f;
		tmpAmbient[1] = 1.0f;
		tmpAmbient[2] = 1.0f;
	}
	else
	{
		tmpAmbient[0] = ambientI[0];
		tmpAmbient[1] = ambientI[1];
		tmpAmbient[2] = ambientI[2];
	}

	if (!diffuseI)
	{
		tmpDiffuse[0] = 1.0f;
		tmpDiffuse[1] = 1.0f;
		tmpDiffuse[2] = 1.0f;
	}
	else
	{
		tmpDiffuse[0] = diffuseI[0];
		tmpDiffuse[1] = diffuseI[1];
		tmpDiffuse[2] = diffuseI[2];
	}

	if (!specularI)
	{
		tmpSpecular[0] = 1.0f;
		tmpSpecular[1] = 1.0f;
		tmpSpecular[2] = 1.0f;
	}
	else
	{
		tmpSpecular[0] = specularI[0];
		tmpSpecular[1] = specularI[1];
		tmpSpecular[2] = specularI[2];
	}

	m = new Material(id, this, shaderId.c_str(), shaderType, tmpAmbient, tmpDiffuse, tmpSpecular, shininess, opacity, shadingModel, textures);
	this->materials.push_back(m);
}



void SceneManager::setDefaultMaterial(int num)
{
	this->defaultMaterial = num;
}



Material* SceneManager::getDefaultMaterial()
{
	return this->materials[defaultMaterial];
}



Material* SceneManager::getMaterial(int material)
{
	return this->materials[material];
}



Material* SceneManager::getMaterial(const char* id)
{
	for (int i = 0; i < materials.size(); i++)
	{
		if (strcmp(materials[i]->getId(), id) == 0)
			return materials[i];
	}

	return nullptr;
}



int SceneManager::getMaterialNum(const char* id)
{
	for (int i = 0; i < materials.size(); i++)
	{
		if (strcmp(materials[i]->getId(), id) == 0)
			return i;
	}

	return -1;
}



int SceneManager::getMaterialNum(Material* material)
{
	for (int i = 0; i < materials.size(); i++)
	{
		if (materials[i] == material)
			return i;
	}

	return -1;
}



Shader* SceneManager::getShader(int shader)
{
	return shaders[shader];
}



Shader* SceneManager::getShader(const char* id)
{
	for (int i = 0; i < shaders.size(); i++)
	{
		if (strcmp(shaders[i]->id, id) == 0)
			return shaders[i];
	}

	return nullptr;
}



int SceneManager::getShaderNum(const char* id)
{
	for (int i = 0; i < shaders.size(); i++)
	{
		if (strcmp(shaders[i]->id, id) == 0)
			return i;
	}

	return -1;
}



int SceneManager::getShaderNum(Shader* shader)
{
	for (int i = 0; i < shaders.size(); i++)
	{
		if (shaders[i] == shader)
			return i;
	}

	return -1;
}



void SceneManager::addTexture(Texture* texture)
{
	this->textures.push_back(texture);
}



Texture* SceneManager::getTexture(int texture)
{
	return textures[texture];
}



Texture* SceneManager::getTexture(const char* id)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (strcmp(textures[i]->id, id) == 0)
			return textures[i];
	}

	return nullptr;
}



int SceneManager::getTextureNum(const char* id)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (strcmp(textures[i]->id, id) == 0)
			return i;
	}

	return -1;
}



int SceneManager::getTextureNum(Texture* texture)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (textures[i] == texture)
			return i;
	}

	return -1;
}



void SceneManager::addModel(const char* id, const char* model)
{
	if(id)
	{
		Model *mod = nullptr;
		for(auto m : models)
		{
			if (strcmp(m->getId(), id) == 0)
				mod = m;
		}

		if(!mod)
		{
			mod = new Model(id, this, model);
			models.push_back(mod);
		}
	}
}



Model* SceneManager::getModel(int model)
{
	return models[model];
}



Model* SceneManager::getModel(const char* id)
{
	for (int i = 0; i < models.size(); i++)
	{
		if (strcmp(models[i]->getId(), id) == 0)
			return models[i];
	}

	return nullptr;
}



int SceneManager::getModelNum(const char* id)
{
	for (int i = 0; i < models.size(); i++)
	{
		if (strcmp(models[i]->getId(), id) == 0)
			return i;
	}

	return -1;
}



int SceneManager::getModelNum(Model* model)
{
	for (int i = 0; i < models.size(); i++)
	{
		if (models[i] == model)
			return i;
	}

	return -1;
}



Entity* SceneManager::createEntity(const char *id, const char *modelId)
{
	return new Entity(id, this, modelId);
}



Light* SceneManager::createLight(const char* id, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, GLfloat constant, GLfloat linear, GLfloat quadratic, const char* modelId)
{
	auto l = new Light(id, this, ambient, diffuse, specular, constant, linear, quadratic, modelId);
	lights.push_back(l);
	return l;
}



void SceneManager::createDirectionalLight(const char* id, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* direction)
{
	delete directionalLight;
	this->directionalLight = new Light("directional", this, ambient, diffuse, specular, direction);
}



vector<Light*> SceneManager::getActiveLights()
{
	auto a_l = vector<Light*>();
	for (auto i = 0; i < lights.size(); i++)
	{
		if(lights[i]->getParent())
		{
			a_l.push_back(lights[i]);
		}
	}

	return a_l;
}



Light* SceneManager::getDirectionalLight()
{
	return directionalLight;
}



SceneNode* SceneManager::getRoot()
{
	return root;
}



void SceneManager::update(float millis)
{
	root->update(millis);
}



void SceneManager::render(Camera *camera)
{
	auto defMaterial = this->getMaterial(defaultMaterial);

	if (!defMaterial)
		return;

	auto mat = glm::mat4();
	root->display(mat, defaultMaterial, camera);
}



void SceneManager::getRenderNodes()
{
}