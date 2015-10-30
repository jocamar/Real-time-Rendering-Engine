#ifndef NODES_H
#define NODES_H

#include "Primitivas.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include "Entity.h"

#include<iostream>
#include <vector>
using namespace std;
class Nodes
{
public:
	const char *id;
	const char *appRef;
	const char *animacao;
	Entity* entity;
	vector<const char*> nodeChildsRef;
	glm::mat4 transf;
	bool dL;
	vector<Primitivas*> prims;
	Nodes() {}
	Nodes(const char *id,const char *appRef,const char *animRef,vector<const char*> nodeChildsRef,vector<Primitivas*> prims,glm::mat4 transf, Entity * entity, bool dL);
	void display(GLint transfloc);
	~Nodes(void);
};
#endif

