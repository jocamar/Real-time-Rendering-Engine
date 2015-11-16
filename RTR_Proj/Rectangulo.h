#pragma once
#include "Primitivas.h"
#include <iostream>

using namespace std;

class Rectangulo:public Primitivas
{
public:
	float x1,x2,y1,y2;
	void draw();
	Rectangulo(float x1,float y1,float x2,float y2);
	~Rectangulo(void);
	void changeTime();
};

