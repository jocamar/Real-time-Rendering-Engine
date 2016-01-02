#include "Light.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Mesh.h"

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
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Light::display(glm::mat4 transf, int material, Camera* camera, bool shadowMap)
{
	if (shadowMap && !this->shadowCaster)
		return;

	for(auto se : subEntities)
	{
		se.mesh->display(transf, material, camera, shadowMap);
	}
}



bool Light::isLeaf()
{
	return true;
}



void Light::generateShadowMap()
{
	glCullFace(GL_FRONT);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (this->directional)
		this->cam = new Camera((glm::vec3(0) - glm::vec3(direction[0], direction[1], direction[2]))*10.f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(direction[0], direction[1], direction[2]), true);

	glm::mat4 view;
	view = cam->GetViewMatrix();
	glm::mat4 projection;
	if (!cam->Ortho)
		projection = glm::perspective(cam->Zoom, (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, 5.0f, 50.0f);
	else
		projection = glm::ortho(-10.f, 10.f, -10.f, 10.f, 5.0f, 100.0f);
	this->lightSpaceMatrix = projection * view;
	cam->ViewProjMatrix = lightSpaceMatrix;

	manager->render(cam, true);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);

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