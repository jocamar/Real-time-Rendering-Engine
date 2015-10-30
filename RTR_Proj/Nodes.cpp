#include "Nodes.h"


Nodes::Nodes(const char *id,const char *appRef,const char *animRef,vector<const char*> nodeChildsRef,vector<Primitivas*> prims,glm::mat4 transf, Entity* entity, bool dL)
{
	this->id=id;
	this->appRef=appRef;
	this->animacao=animRef;
	this->nodeChildsRef=nodeChildsRef;
	this->prims=prims;
	this->transf = transf;
	this->entity = entity;
	this->dL=dL;
}

void Nodes::display(GLint transfLoc) {
	glUniformMatrix4fv(transfLoc, 1, GL_FALSE, glm::value_ptr(transf));
	entity->display();
	
}


Nodes::~Nodes(void)
{
}
