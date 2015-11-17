#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>
#include <vector>
#include "Cylinder.h"

using namespace std;


Cylinder::Cylinder(string id) {

	this->idMesh = id;


	enum { Vertices, Textures, Colors, Indexes, numVBO };

	float radius = 2.0f, stacks = 100;
	int slices = 100;


	long float pi = acos(-1.0);
	long float angulo = 360.0 / slices;
	float ki, kf;

/*	for (int i = 0; i < slices; i++) {
		vertices.push_back(cos(angulo*(i)*pi / 180));
		vertices.push_back(sin(angulo*(i)*pi / 180));
		vertices.push_back(0);
	} */

	for (int i = 0; i < slices; i++) {
		ki = 0.0;
		kf = 1.0 / (float)stacks;
		for (int j = 0; j < stacks; j++){

			vertices.push_back(cos(angulo*(i)*pi / 180));
			vertices.push_back(sin(angulo*(i)*pi / 180));
			vertices.push_back(ki);

			vertices.push_back(cos(angulo*(i + 1)*pi / 180));
			vertices.push_back(sin(angulo*(i + 1)*pi / 180));
			vertices.push_back(ki);

			vertices.push_back(cos(angulo*(i + 1)*pi / 180));
			vertices.push_back(sin(angulo*(i + 1)*pi / 180));
			vertices.push_back(kf);

			vertices.push_back(cos(angulo*(i)*pi / 180));
			vertices.push_back(sin(angulo*(i)*pi / 180));
			vertices.push_back(kf);


			ki = kf;
			kf += 1.0 / (float)stacks;

		}
	}

	/*for (int i = 0; i < slices; ++i) {
		
		vertices.push_back(cos(angulo*(i)*pi / 180));
		vertices.push_back(sin(angulo*(i)*pi / 180));
		vertices.push_back(0);

	} */



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


	indices.resize(slices * stacks * 4);
	std:vector<GLushort>::iterator i = indices.begin();
	for (int r = 0; r < slices; r++) for (int s = 0; s < stacks; s++) {
		*i++ = r * slices + s;
		*i++ = r * slices + (s + 1);
		*i++ = (r + 1) * slices + (s + 1);
		*i++ = (r + 1) * slices + s;
	}




	GLuint buffers[numVBO];
	glGenVertexArrays(1, &VAO);
	glGenBuffers(numVBO, buffers);

	glBindVertexArray(VAO);

	//BIND VERTICES
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);



}

void Cylinder::display() {
	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}