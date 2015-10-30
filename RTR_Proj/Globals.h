#pragma once

#include <vector>
using std::vector;

class Globals
{
public:
	vector<float> background;
	const char *drawmode;
	const char *shading;
	const char *cullface;
	const char *cullorder;
	Globals(vector<float> background,const char *drawmode,const char *shading,const char *cullface,const char *cullorder);
	~Globals(void);
};
