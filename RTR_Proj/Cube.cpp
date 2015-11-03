#include "Cube.h"

Cube::Cube(string id) {

	this->idMesh = id;


	enum { Vertices, Textures, Colors, Indexes, numVBO};

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f
	};

	GLfloat texVertices[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	GLfloat colors[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.1f,
		0.0f, 0.1f, 0.0f,
		0.0f, 0.1f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f
	};

	GLubyte cubeIndexes[] = {
		0, 1, 3, 2,
		4, 6, 7, 5,
		2, 3, 7, 6,
		0, 4, 5, 1,
		0, 2, 6, 4,
		1, 5, 7, 3
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndexes), cubeIndexes, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);



}

void Cube::display() {
	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);
}