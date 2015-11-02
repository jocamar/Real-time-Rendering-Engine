#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>
#include <vector>
#include "Cylinder.h"

using namespace std;


Cylinder::Cylinder(string id) {

	this->nverts = 0;
	this->idMesh = id;


	enum { Vertices, Textures, Colors, Indexes, numVBO };

	float radius = 1.0f, halfLength = 1.0f;
	int slices = 5;
	
	vector<GLfloat> verts;


	for (int i = 0; i<slices; i++) {
		float theta = ((float)i)*2.0*M_PI;
		float nextTheta = ((float)i + 1)*2.0*M_PI;
		

		verts.push_back(0.0f);
		verts.push_back(halfLength);
		verts.push_back(0.0f);

		verts.push_back(radius*cos(theta));
		verts.push_back(halfLength);
		verts.push_back(radius*sin(theta));

		verts.push_back(radius*cos(nextTheta));
		verts.push_back(halfLength);
		verts.push_back(radius*sin(nextTheta));

		verts.push_back(radius*cos(nextTheta));
		verts.push_back(-halfLength);
		verts.push_back(radius*sin(nextTheta));

		verts.push_back(radius*cos(theta));
		verts.push_back(-halfLength);
		verts.push_back(radius*sin(theta));

		verts.push_back(0.0f);
		verts.push_back(-halfLength);
		verts.push_back(0.0f);
	}

	GLfloat vertices[256];
	this->nverts = verts.size();

	for (int x = 0; x < verts.size(); x++)
	{
		vertices[x] = verts.at(x);
	}




	GLfloat texVertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f
	};

	GLfloat colors[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.1f, 0.1f,
		0.0f, 0.1f, 0.0f,
	};

	GLubyte cylinderIndexes[] = {
		0, 1, 2
	};

	GLuint buffers[numVBO];
	glGenVertexArrays(1, &VAO);
	glGenBuffers(numVBO, buffers);

	glBindVertexArray(VAO);

	//BIND VERTICES
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*glVertexPointer(3, GL_FLOAT, 0, (GLvoid*) 0);
	glEnableClientState(GL_VERTEX_ARRAY);*/

	//BIND TEXTURES
	/*glBindBuffer(GL_ARRAY_BUFFER, buffers[Textures]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texVertices), texVertices, GL_STATIC_DRAW);
	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*) 0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//BIND COLORS
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glColorPointer(3, GL_FLOAT, 0, (GLvoid*) 0);
	glEnableClientState(GL_COLOR_ARRAY);*/

	//BIND INDEXES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Indexes]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinderIndexes), cylinderIndexes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);



}

void Cylinder::display() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, nverts, GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);
}