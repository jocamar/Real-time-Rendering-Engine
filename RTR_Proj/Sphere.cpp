#define _USE_MATH_DEFINES

#include <math.h>
#include <iostream>
#include <vector>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sphere.h"

using namespace std;


Sphere::Sphere(string id) {

	this->nverts = 0;
	this->idMesh = id;


	enum { Vertices, Textures, Colors, Indexes, numVBO };

	float radius = 3.0f, halfLength = 5.0f;
	int slices = 5;
	int stacks = 2;

	/*vector<GLfloat> verts;


	for (int i = 0; i<slices; i++) {
		float V = i / (float)stacks;
		float phi = V * glm::pi <float>();

		// Loop Through Slices
		for (int j = 0; j <= slices; ++j){

			float U = j / (float)slices;
			float theta = U * (glm::pi <float>() * 2);

			// Calc The Vertex Positions
			float x = cosf(theta) * sinf(phi);
			float y = cosf(phi);
			float z = sinf(theta) * sinf(phi);

			// Push Back Vertex Data
			verts.push_back(x * radius);
			verts.push_back(y * radius);
			verts.push_back(z * radius);
		}
	}*/
	vector<GLfloat> vertices;
	vector<GLfloat> indices;
	int i, j;
	int lats = 40;
	int longs = 40;
	int indicator = 0;
	for (i = 0; i <= lats; i++) {
		double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);

		double lat1 = glm::pi<double>() * (-0.5 + (double)i / lats);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		for (j = 0; j <= longs; j++) {
			double lng = 2 * glm::pi<double>() * (double)(j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);

			vertices.push_back(x * zr0);
			vertices.push_back(y * zr0);
			vertices.push_back(z0);
			indices.push_back(indicator);
			indicator++;

			vertices.push_back(x * zr1);
			vertices.push_back(y * zr1);
			vertices.push_back(z1);
			indices.push_back(indicator);
			indicator++;
		}
		indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
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

	this->nverts = indices.size();

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLfloat), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);


}

void Sphere::display() {
	glBindVertexArray(VAO);
	glDrawElements(GL_QUAD_STRIP, nverts, GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);
}