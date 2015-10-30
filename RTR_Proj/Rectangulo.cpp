#include "Rectangulo.h"


void Rectangulo::draw()
{
	glPushMatrix();
		glBegin(GL_QUADS); 
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(x1,y1); 
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(x2,y1); 
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(x2,y2); 
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(x1,y2); 
		glEnd();
	glPopMatrix();
}

Rectangulo::Rectangulo(float x1,float y1,float x2,float y2)
{
		this->x1=x1;
		this->x2=x2;
		this->y1=y1;
		this->y2=y2;
}

void Rectangulo::changeTime()
{

}


Rectangulo::~Rectangulo(void)
{
}
