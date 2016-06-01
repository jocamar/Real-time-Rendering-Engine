#include "Material.h"
#include "Camera.h"
#include "Scene Graph/SceneManager.h"

Material::Material(const char *id, SceneManager *manager, const char *shaderId, shaderTypes shaderType, GLfloat* ambientI, GLfloat* diffuseI,
						GLfloat *specularI, GLfloat shininess, GLfloat opacity, int shadingModel, const char *diffuseId, const char *specularId, const char *normalId)
{
	this->id = id;
	this->shader = manager->getShader(shaderId);
	this->shaderType = shaderType;
	this->manager = manager;
	
	this->ambientIntensity = ambientI;
	this->diffuseIntensity = diffuseI;
	this->specularIntensity = specularI;
	this->shininess = shininess;
	this->opacity = opacity;
	this->shadingModel = shadingModel;
	
	if(diffuseId)
		textures.push_back(manager->getTexture(diffuseId));

	if(specularId)
		textures.push_back(manager->getTexture(specularId));

	if (normalId)
		textures.push_back(manager->getTexture(normalId));

	glGenTextures(1, &emissionMap);

	if(opacity <= 0.9)
	{
		this->transparent = true;
	}
	else
	{
		this->transparent = false;
	}
}



Material::Material(const char* id, SceneManager* manager, const char* shaderId, shaderTypes shaderType, GLfloat* ambientI, GLfloat* diffuseI,
						GLfloat *specularI, GLfloat shininess, GLfloat opacity, int shadingModel, vector<Texture*> textures)
{
	this->id = id;
	this->shader = manager->getShader(shaderId);
	this->shaderType = shaderType;
	this->manager = manager;

	this->ambientIntensity = ambientI;
	this->diffuseIntensity = diffuseI;
	this->specularIntensity = specularI;
	this->shininess = shininess;
	this->opacity = opacity;
	this->shadingModel = shadingModel;

	this->textures = textures;

	glGenTextures(1, &emissionMap);

	if (opacity <= 0.9)
	{
		this->transparent = true;
	}
	else
	{
		this->transparent = false;
	}
}

Material::Material(const char *id, SceneManager *manager, const char *shaderId, shaderTypes shaderType, GLfloat *ambientI, GLfloat *diffuseI, GLfloat *specularI, GLfloat shininess,
	GLfloat opacity, int shadingModel, vector<const GLchar*> faces) {

	this->id = id;
	this->shader = manager->getShader(shaderId);
	this->shaderType = shaderType;
	this->manager = manager;

	this->ambientIntensity = ambientI;
	this->diffuseIntensity = diffuseI;
	this->specularIntensity = specularI;
	this->shininess = shininess;
	this->opacity = opacity;
	this->shadingModel = shadingModel;

	this->cubemapTexture = loadCubemap(faces);

	if (opacity <= 0.9)
	{
		this->transparent = true;
	}
	else
	{
		this->transparent = false;
	}

}



Material::~Material()
{
	glDeleteTextures(1, &emissionMap);
}



const char* Material::getId()
{
	return id;
}



Shader* Material::getShader()
{
	return shader;
}



vector<Texture*> Material::getDiffuseMaps()
{
	vector<Texture*> rtrn;
	for(auto t : textures)
	{
		if(t->type == "diffuse_texture")
		{
			rtrn.push_back(t);
		}
	}
	return rtrn;
}



vector<Texture*> Material::getSpecMaps()
{
	vector<Texture*> rtrn;
	for (auto t : textures)
	{
		if (t->type == "specular_texture")
		{
			rtrn.push_back(t);
		}
	}
	return rtrn;
}



GLuint Material::getEmissionMap()
{
	return emissionMap;
}



bool Material::isTransparent()
{
	return transparent;
}


//@TODO reduce gl calls by caching the uniform locations
void Material::use(Camera *camera, bool shadowMap, Globals::LIGHT_TYPE shadowType)
{
	//If we're rendering the normal scene
	if(!shadowMap)
	{
		//Textured has no shadows
		if(shaderType == TEXTURED)
		{
			//Setup shader arguments
			glUniform1f(glGetUniformLocation(shader->Program, "material.shininess"), this->shininess);
			glUniform1f(glGetUniformLocation(shader->Program, "material.opacity"), this->opacity);

			GLuint diffuseNr = 1;
			GLuint specularNr = 1;
			GLuint normalNr = 1;
			for (GLuint i = 0; i < this->textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
			
				stringstream ss;
				string number;
				string name = this->textures[i]->type;
				if (name == "diffuse_texture")
					ss << diffuseNr++; // Transfer GLuint to stream
				else if (name == "specular_texture")
				{
					ss << specularNr++; // Transfer GLuint to stream
				}
				else if (name == "normal_texture")
				{
					ss << normalNr++;
				}
				number = ss.str();

				//Set this texture as active in the shader
				glUniform1i(glGetUniformLocation(shader->Program, ("material." + name + number + "_active").c_str()), 1);
				glUniform1i(glGetUniformLocation(shader->Program, ("material." + name + number).c_str()), i);
				glBindTexture(GL_TEXTURE_2D, this->textures[i]->texture);
			}

			glUniform1i(glGetUniformLocation(shader->Program, "material.shading_model"), this->shadingModel);

			if (ambientIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "material.ambientI"), ambientIntensity[0], ambientIntensity[1], ambientIntensity[2]);
			}

			if (diffuseIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "material.diffuseI"), diffuseIntensity[0], diffuseIntensity[1], diffuseIntensity[2]);
			}

			if (specularIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "material.specularI"), specularIntensity[0], specularIntensity[1], specularIntensity[2]);
			}
		}

		//Lighting textured has textures and shadows
		if (shaderType == LIGHTING_TEXTURED)
		{
			//Setup shader arguments

			GLint viewPosLoc = glGetUniformLocation(shader->Program, "viewPos");
			glUniform3f(viewPosLoc, camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

			glUniform1f(glGetUniformLocation(shader->Program, "material.shininess"), this->shininess);
			glUniform1f(glGetUniformLocation(shader->Program, "material.opacity"), this->opacity);

			GLuint diffuseNr = 1;
			GLuint specularNr = 1;
			GLuint normalNr = 1;
			for (GLuint i = 0; i < this->textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding

				stringstream ss;
				string number;
				string name = this->textures[i]->type;
				if (name == "diffuse_texture")
					ss << diffuseNr++; // Transfer GLuint to stream
				else if (name == "specular_texture")
				{
					ss << specularNr++; // Transfer GLuint to stream
				}
				else if (name == "normal_texture")
				{
					ss << normalNr++;
				}
				number = ss.str();

				//Set this texture as active
				glUniform1i(glGetUniformLocation(shader->Program, ("material." + name + number + "_active").c_str()), 1);
				glUniform1i(glGetUniformLocation(shader->Program, ("material." + name + number).c_str()), i);
				glBindTexture(GL_TEXTURE_2D, this->textures[i]->texture);
			}

			glUniform1i(glGetUniformLocation(shader->Program, "material.shading_model"), this->shadingModel);

			if (ambientIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "material.ambientI"), ambientIntensity[0], ambientIntensity[1], ambientIntensity[2]);
			}

			if (diffuseIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "material.diffuseI"), diffuseIntensity[0], diffuseIntensity[1], diffuseIntensity[2]);
			}

			if (specularIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "material.specularI"), specularIntensity[0], specularIntensity[1], specularIntensity[2]);
			}

			auto directionalLight = this->manager->getDirectionalLight();

			GLfloat* dif;
			GLfloat* amb;
			GLfloat* spec;
			GLfloat* dir;

			//if we have a directional light set it up on the shader
			if (directionalLight) {
				dif = directionalLight->getDiffuse();
				amb = directionalLight->getAmbient();
				spec = directionalLight->getSpecular();
				dir = directionalLight->getDirection();

				glUniform3f(glGetUniformLocation(shader->Program, "dirLight.direction"), dir[0], dir[1], dir[2]);
				glUniform3f(glGetUniformLocation(shader->Program, "dirLight.ambient"), amb[0], amb[1], amb[2]);
				glUniform3f(glGetUniformLocation(shader->Program, "dirLight.diffuse"), dif[0], dif[1], dif[2]);
				glUniform3f(glGetUniformLocation(shader->Program, "dirLight.specular"), spec[0], spec[1], spec[2]);
				glUniformMatrix4fv(glGetUniformLocation(shader->Program, "dirLight.lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(*directionalLight->getLightSpaceMatrix()));

				//Activate sixth texture unit, bind the texture and set it up on the shader
				glActiveTexture(GL_TEXTURE6);
				glUniform1i(glGetUniformLocation(shader->Program, "dirLight.shadowMap"), 6);
				glBindTexture(GL_TEXTURE_2D, directionalLight->getShadowMap());
			}

			auto lights = this->manager->getActiveLights();

			glUniform1i(glGetUniformLocation(shader->Program, "activeLights"), lights.size());

			//If we have point lights set them up too
			int i = 0;
			for (; i < MAX_LIGHTS && i < lights.size(); i++)
			{
				auto l = lights[i];
				auto pos = l->getParent()->getWorldPosition();
				dif = l->getDiffuse();
				amb = l->getAmbient();
				spec = l->getSpecular();
				auto constant = l->getConstant();
				auto linear = l->getLinear();
				auto quadratic = l->getQuadratic();

				glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].position").c_str()), pos.x, pos.y, pos.z);
				glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].ambient").c_str()), amb[0], amb[1], amb[2]);
				glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].diffuse").c_str()), dif[0], dif[1], dif[2]);
				glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].specular").c_str()), spec[0], spec[1], spec[2]);
				glUniform1f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].constant").c_str()), constant);
				glUniform1f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].linear").c_str()), linear);
				glUniform1f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].quadratic").c_str()), quadratic);

				glUniform1f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].far_plane").c_str()), 45.0f);

				//Shadow maps for point lights start at the 7th texture unit
				glActiveTexture(GL_TEXTURE7+i);
				glUniform1i(glGetUniformLocation(shader->Program, ("shadowMap" + to_string(i)).c_str()), 7 + i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, l->getCubeShadowMap());
			}
			for (; i < MAX_LIGHTS; i++)
			{
				glActiveTexture(GL_TEXTURE7 + i);
				glUniform1i(glGetUniformLocation(shader->Program, ("shadowMap" + to_string(i)).c_str()), 7 + i);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}


			glActiveTexture(GL_TEXTURE0);
		}
		//Emitter shaders just color the object according to the diffuse light color it emits
		else if (shaderType == EMITTER)
		{
			if (diffuseIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "diffuse"), diffuseIntensity[0], diffuseIntensity[1], diffuseIntensity[2]);
			}
			else
			{
				glUniform3f(glGetUniformLocation(shader->Program, "diffuse"), 1.0, 1.0, 1.0);
			}
		}
		//Reflective shaders are currently not used
		//@TODO check if this needs a shader at all or if it should be a parameter in another shader
		else if (shaderType == REFLECTIVE)
		{
			glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
			glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

			glActiveTexture(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(shader->Program, "skybox"), 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

		}
		//Particle shaders are also not used
		else if (shaderType == PARTICLE)
		{
			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textures[0]->texture);
			glUniform1i(glGetUniformLocation(shader->Program,"texture"),0);

			glUniform3f(glGetUniformLocation(shader->Program, "camera_right"), camera->getRight().x, camera->getRight().y, camera->getRight().z);
			glUniform3f(glGetUniformLocation(shader->Program, "camera_up"), camera->getUp().x, camera->getUp().y, camera->getUp().z);

			glm::mat4 viewProjection = camera->GetViewMatrix() * camera->GetProjectionMatrix();

			glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view_proj_matrix"), 1, GL_FALSE, glm::value_ptr(viewProjection));
		}
	}
	
}

void Material::unUse(Camera* camera, bool shadowMap, Globals::LIGHT_TYPE shadowType)
{
	if(!shadowMap)
	{
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		GLuint normalNr = 1;

		//Reset the previously bound textures on the texture units
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
						
			stringstream ss;
			string number;
			string name = this->textures[i]->type;
			if (name == "diffuse_texture")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "specular_texture")
			{
				ss << specularNr++; // Transfer GLuint to stream
			}
			else if (name == "normal_texture")
			{
				ss << normalNr++;
			}
			number = ss.str();

			glBindTexture(GL_TEXTURE_2D, NULL);
			glUniform1i(glGetUniformLocation(shader->Program, ("material." + name + number + "_active").c_str()), 0);
		}
		glActiveTexture(GL_TEXTURE0);
	}

}

GLuint Material::loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	//Bind the cubemap, load the images and setup texture parametes
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}