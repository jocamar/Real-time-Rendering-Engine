#include "Rectangle.h"


void Rectangle::draw() {
	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);




}

Rectangle::Rectangle(float x1, float y1, float x2, float y2, GLuint VBO)
{
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	this->VBO = VBO;
}

void Rectangle::changeTime()
{

}


Rectangle::~Rectangle(void)
{
}
