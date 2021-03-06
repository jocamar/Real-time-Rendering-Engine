#include "Scene Graph/SceneManager.h"

SceneManager::SceneManager()
{
	this->root = new SceneNode("_root", this);
	this->materials = vector<Material*>();

	//Load default shaders to render shadow maps
	this->shadowShader = new Shader("Resources/Shaders/shadowShader.vs", "Resources/Shaders/shadowShader.frag");
	this->omniShadowShader = new Shader("Resources/Shaders/omniShadowShader.vs", "Resources/Shaders/omniShadowShader.frag", "Resources/Shaders/omniShadowShader.gs");

	this->orderUpdatedDirectional = false;
	this->orderUpdatedPoint = false;
	this->orderUpdatedRegular = false;

	this->currentShader = nullptr;
	this->currentMaterial = -1;
}



void SceneManager::addMaterial(const char *id, const char *vert, const char *frag, char *diffuse, char *specular, char *normal, GLfloat *ambientI,
										GLfloat *diffuseI, GLfloat *specularI, GLfloat shininess, GLfloat opacity, int shadingModel,
										Material::shaderTypes shaderType, GLint interpMethod)
{
	Material *m = this->getMaterial(id);

	//Check if material is already registered
	if (m) return;

	GLfloat* tmpAmbient = new GLfloat[3];
	GLfloat* tmpDiffuse = new GLfloat[3];
	GLfloat* tmpSpecular = new GLfloat[3];

	string shaderId = vert;
	shaderId += frag;

	Shader *shader = nullptr;
	//Search the shaders to see if it's already loaded
	for(auto s : shaders)
	{
		if(strcmp(s->id,shaderId.c_str()) == 0)
		{
			shader = s;
			break;
		}
	}

	//Otherwise load it
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
		//Search textures to see if its already loaded
		for (auto t : textures)
		{
			if (strcmp(t->id, diffuse) == 0)
			{
				texture = t;
				break;
			}
		}

		//Otherwise load it
		//@TODO move all image loading to a media manager?
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
		//Search textures to see if its already loaded
		for (auto t : textures)
		{
			if (strcmp(t->id, specular) == 0)
			{
				texture = t;
				break;
			}
		}

		//Otherwise load it
		//@TODO move all image loading to a media manager?
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
		//Search textures to see if its already loaded
		for (auto t : textures)
		{
			if (strcmp(t->id, normal) == 0)
			{
				texture = t;
				break;
			}
		}

		//Otherwise load it
		//@TODO move all image loading to a media manager?
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
	
	//Setup shading parameters
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

	//Create material and register it
	m = new Material(id, this, shaderId.c_str(), shaderType, tmpAmbient, tmpDiffuse, tmpSpecular, shininess, opacity, shadingModel, diffuse, specular, normal);
	this->materials.push_back(m);
}



void SceneManager::addMaterial(const char *id, const char *vert, const char *frag, vector<Texture*> textures, GLfloat *ambientI, GLfloat *diffuseI, 
									GLfloat *specularI, GLfloat shininess, GLfloat opacity, int shadingModel, Material::shaderTypes shaderType)
{
	Material *m = this->getMaterial(id);

	//Check if material is already registered
	if (m) return;

	GLfloat* tmpAmbient = new GLfloat[3];
	GLfloat* tmpDiffuse = new GLfloat[3];
	GLfloat* tmpSpecular = new GLfloat[3];

	string shaderId = vert;
	shaderId += frag;

	//Search the shaders to see if it's already loaded
	Shader *shader = nullptr;
	for (auto s : shaders)
	{
		if (strcmp(s->id, shaderId.c_str()) == 0)
		{
			shader = s;
			break;
		}
	}

	//Otherwise load it
	if (!shader)
	{
		shader = new Shader(vert, frag);
		shaders.push_back(shader);
	}

	//Setup shading parameters
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

	//Create material and register it
	m = new Material(id, this, shaderId.c_str(), shaderType, tmpAmbient, tmpDiffuse, tmpSpecular, shininess, opacity, shadingModel, textures);
	this->materials.push_back(m);
}

void SceneManager::addMaterial(const char *id, const char *vert, const char *frag, vector<const GLchar*> faces, GLfloat *ambientI, GLfloat *diffuseI,
	GLfloat *specularI, GLfloat shininess, GLfloat opacity, int shadingModel, Material::shaderTypes shaderType)
{
	Material *m = this->getMaterial(id);
	//Check if material is already registered
	if (m) return;

	GLfloat* tmpAmbient = new GLfloat[3];
	GLfloat* tmpDiffuse = new GLfloat[3];
	GLfloat* tmpSpecular = new GLfloat[3];

	string shaderId = vert;
	shaderId += frag;

	//Search the shaders to see if it's already loaded
	Shader *shader = nullptr;
	for (auto s : shaders)
	{
		if (strcmp(s->id, shaderId.c_str()) == 0)
		{
			shader = s;
			break;
		}
	}

	//Otherwise load it
	if (!shader)
	{
		shader = new Shader(vert, frag);
		shaders.push_back(shader);
	}

	//Setup shading parameters
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

	//Create material and register it
	m = new Material(id, this, shaderId.c_str(), shaderType, tmpAmbient, tmpDiffuse, tmpSpecular, shininess, opacity, shadingModel, faces);
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

		//Check if model exists, if not create it and register it
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



Entity* SceneManager::createEntity(const char *id, const char *modelId, bool shadowCaster, bool transparent)
{
	return new Entity(id, this, modelId, nullptr, shadowCaster, transparent);
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



void SceneManager::render(Camera *camera, bool shadowMap, Globals::LIGHT_TYPE shadowType)
{
	//Check if render orders need to be updated and update them if need be.
	//@TODO check if this is needed
	if (!orderUpdatedDirectional && shadowMap && shadowType == Globals::DIRECTIONAL)
	{
		this->directionalShadowOrder = this->getRenderEntities(camera, shadowMap, shadowType);
		//this->orderUpdatedDirectional = true;
	}
	else if (!orderUpdatedPoint && shadowMap && shadowType == Globals::POINT)
	{
		this->pointShadowOrder = this->getRenderEntities(camera, shadowMap, shadowType);
		//this->orderUpdatedPoint = true;
	} 
	else if (!orderUpdatedRegular && !shadowMap)
	{
		this->regularOrder = this->getRenderEntities(camera, shadowMap, shadowType);
		//this->orderUpdatedRegular = true;
	}

	RenderOrder *order;
	//Select correct render order
	if (!shadowMap)
		order = &(this->regularOrder);
	else if (shadowMap && shadowType == Globals::DIRECTIONAL)
		order = &(this->directionalShadowOrder);
	else order = &(this->pointShadowOrder);

	this->currentShader = nullptr;
	this->currentMaterial = -1;

	//Go through every sub-entity in order and display them
	for(auto se : order->Entities)
	{
		//Fetch material and shader
		auto mat = getMaterial(se->materialToUse);
		auto s = mat->getShader();

		//Check if this sub-entity uses a different material than the previous one (entities are ordered by material)
		if (currentMaterial != se->materialToUse && currentMaterial >= 0)
		{
			//If so get the new material and unUse the previous one before we change the shader
			auto curr_mat = getMaterial(currentMaterial);
			curr_mat->unUse(camera, shadowMap, shadowType);
		}

		if (!shadowMap && currentShader != s)
		{
			//If we're not rendering for shadow maps (which use the same shader previously set always)
			//And the shader has changed use the new shader
			s->Use();
			currentShader = s;
		}

		//Even though we already checked for this above we need to use the material after the shader is used
		//Because the use function doesn't set up the shader
		if (currentMaterial != se->materialToUse)
		{
			mat->use(camera, shadowMap, shadowType);
			currentMaterial = se->materialToUse;
		}

		//Display the mesh
		se->mesh->display(se->entity->getParent()->getTransfMatrix(), se->materialToUse, camera, shadowMap, shadowType);
	}

	//UnUse any material we had used before.
	if (currentMaterial >= 0)
	{
		auto curr_mat = getMaterial(currentMaterial);
		curr_mat->unUse(camera, shadowMap, shadowType);
	}
}



void SceneManager::generateShadowMaps(Camera *camera)
{
	//Set up shader for shadow maps
	this->shadowShader->Use();
	//glDisable(GL_CULL_FACE);

	//Cull front facing faces to avoid self shadowing
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//Set up viewport with shadow resolution
	glViewport(0, 0, DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT);

	//Enable depth test for shadows
	glEnable(GL_DEPTH_TEST);

	//Generate shadow map for directional light
	this->directionalLight->generateShadowMap(camera);
	//glCullFace(GL_BACK);
	//glDisable(GL_CULL_FACE);

	//Change shader and viewport for omni-directional lights
	this->omniShadowShader->Use();
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

	//Generate shadow maps for each light
	auto ls = getActiveLights();
	for(auto l : ls)
	{
		l->generateShadowMap(camera);
	}
	//glDisable(GL_CULL_FACE);

	//Reset face culling settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}



RenderOrder SceneManager::getRenderEntities(Camera *camera, bool shadowMap, Globals::LIGHT_TYPE shadowType)
{
	RenderOrder order;
	auto defMaterial = this->getMaterial(defaultMaterial);

	if (!defMaterial)
		return order;

	//Get all render entities and order them
	order = this->root->getRenderEntities(defaultMaterial, camera, shadowMap, shadowType);
	std::sort(order.Entities.begin(), order.Entities.end(), SubEntity::less);
	return order;
}



Shader* SceneManager::getShadowShader()
{
	return shadowShader;
}



Shader* SceneManager::getOmniShadowShader()
{
	return omniShadowShader;
}