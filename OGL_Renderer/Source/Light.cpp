#include "Light.h"
#include "Scene Graph/SceneManager.h"
#include "Camera.h"
#include "Primitives/Mesh.h"

Light::Light(const char *idLight, SceneManager* manager, GLfloat *ambient, GLfloat *diffuse, GLfloat *specular, 
					GLfloat constant, GLfloat linear, GLfloat quadratic, const char *modelId, SceneNode* parent) : Entity(idLight, manager, modelId, parent, false)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->constant = constant;
	this->linear = linear;
	this->quadratic = quadratic;
	this->directional = false;
	this->direction = nullptr;

	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthCubemap);

	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



Light::Light(const char *idLight, SceneManager* manager, GLfloat* ambient, GLfloat* diffuse, GLfloat* specular, GLfloat* direction) : Entity(idLight, manager, nullptr, nullptr, false)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->directional = true;
	this->direction = direction;

	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		DIR_SHADOW_WIDTH, DIR_SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Light::display(int material, Camera* camera, bool shadowMap, Globals::LIGHT_TYPE shadowType)
{
	if (shadowMap && !this->shadowCaster)
		return;

	for(auto se : subEntities)
	{
		se.mesh->display(this->parent->getTransfMatrix(), material, camera, shadowMap, shadowType);
	}
}



bool Light::isLeaf()
{
	return true;
}



void Light::generateShadowMap(Camera *camera)
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (this->directional)
	{
		auto pos = glm::normalize(glm::vec3(0) - glm::vec3(direction[0], direction[1], direction[2]))*50.f;
		this->cam = new Camera(manager,camera->getPosition()+pos, glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(direction[0], direction[1], direction[2]), true, 0.1f, 80.0f, (float)DIR_SHADOW_WIDTH / (float)DIR_SHADOW_HEIGHT);
	}
	else
	{
		glm::vec3 wp = this->parent->getWorldPosition();
		this->cam = new Camera(manager,this->parent->getWorldPosition(), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1, 0, 0), false, 0.01f, 45.0f, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, 90.0f);
		this->cam->Zoom = 90.0f;
	}

	glm::mat4 view, projection;
	view = cam->GetViewMatrix();
	projection = cam->GetProjectionMatrix();

	if (!cam->Ortho)
	{
		this->cubeLightSpaceMatrixes.push_back(projection * glm::lookAt(cam->getPosition(), cam->getPosition() + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		this->cubeLightSpaceMatrixes.push_back(projection * glm::lookAt(cam->getPosition(), cam->getPosition() + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		this->cubeLightSpaceMatrixes.push_back(projection * glm::lookAt(cam->getPosition(), cam->getPosition() + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		this->cubeLightSpaceMatrixes.push_back(projection * glm::lookAt(cam->getPosition(), cam->getPosition() + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		this->cubeLightSpaceMatrixes.push_back(projection * glm::lookAt(cam->getPosition(), cam->getPosition() + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		this->cubeLightSpaceMatrixes.push_back(projection * glm::lookAt(cam->getPosition(), cam->getPosition() + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
		cam->cubeViewProjectionMatrixes = cubeLightSpaceMatrixes;
		manager->render(cam, true, Globals::POINT);
	}
	else
	{
		this->lightSpaceMatrix = projection * view;
		cam->ViewProjMatrix = lightSpaceMatrix;
		manager->render(cam, true);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->cubeLightSpaceMatrixes.clear();
	delete cam;
}



bool Light::isDirectional()
{
	return directional;
}



GLfloat* Light::getAmbient()
{
	return ambient;
}



GLfloat* Light::getDiffuse()
{
	return diffuse;
}



GLfloat* Light::getSpecular()
{
	return specular;
}



GLfloat* Light::getDirection()
{
	return direction;
}



glm::mat4* Light::getLightSpaceMatrix()
{
	return &lightSpaceMatrix;
}



GLuint Light::getShadowMap()
{
	return this->depthMap;
}



GLuint Light::getCubeShadowMap()
{
	return this->depthCubemap;
}



GLfloat Light::getConstant()
{
	return constant;
}



GLfloat Light::getLinear()
{
	return linear;
}



GLfloat Light::getQuadratic()
{
	return quadratic;
}