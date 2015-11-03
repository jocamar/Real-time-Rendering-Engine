#pragma once
#include "Primitivas.h"
#include <GL/glew.h>
#include <iostream>

using namespace std;
class Rectangle :public Primitivas
{
public:
	float x1, x2, y1, y2;
	GLuint VBO;
	void draw();
	Rectangle(float x1, float y1, float x2, float y2, GLuint VBO);
	~Rectangle(void);
	void changeTime();
};

