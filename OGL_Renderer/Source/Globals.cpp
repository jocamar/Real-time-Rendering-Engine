#include "Globals.h"

Globals::Globals(vector<float> background,const char *drawmode,const char *shading,const char *cullface,const char *cullorder)
{
	this->background=background;
	this->drawmode=drawmode;
	this->shading=shading;
	this->cullface=cullface;
	this->cullorder=cullorder;
}


Globals::~Globals(void)
{
}