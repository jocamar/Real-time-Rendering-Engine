//#define _USE_MATH_DEFINES
//
//#include <math.h>
//#include <iostream>
//#include <vector>
//
//// GLM Mathemtics
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "Sphere.h"
//
//using namespace std;
//
//
//Sphere::Sphere(string id) {
//
//	this->idMesh = id;
//
//
//	enum { Vertices, Textures, Colors, Indexes, numVBO };
//
//	float radius = 1.0f, halfLength = 1.0f;
//	int rings = 100;
//	int sectors = 100;
//
//
//	float const R = 1. / (float)(rings - 1);
//	float const S = 1. / (float)(sectors - 1);
//	int r, s;
//
//	vertices.resize(rings * sectors * 3);
//	normals.resize(rings * sectors * 3);
//	texcoords.resize(rings * sectors * 2);
//	std::vector<GLfloat>::iterator v = vertices.begin();
//	std::vector<GLfloat>::iterator n = normals.begin();
//	std::vector<GLfloat>::iterator t = texcoords.begin();
//	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
//		float const y = sin(-M_PI_2 + M_PI * r * R);
//		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
//		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
//
//		*t++ = s*S;
//		*t++ = r*R;
//
//		*v++ = x * radius;
//		*v++ = y * radius;
//		*v++ = z * radius;
//
//		*n++ = x;
//		*n++ = y;
//		*n++ = z;
//	}
//
//	indices.resize(rings * sectors * 4);
//std:vector<GLushort>::iterator i = indices.begin();
//	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
//		*i++ = r * sectors + s;
//		*i++ = r * sectors + (s + 1);
//		*i++ = (r + 1) * sectors + (s + 1);
//		*i++ = (r + 1) * sectors + s;
//	}
//
//	GLuint buffers[numVBO];
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(numVBO, buffers);
//
//	glBindVertexArray(VAO);
//
//	//BIND VERTICES
//	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices]);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat) , &vertices[0], GL_STATIC_DRAW);
//	/*glVertexPointer(3, GL_FLOAT, 0, (GLvoid*) 0);
//	glEnableClientState(GL_VERTEX_ARRAY);*/
//
//	//BIND TEXTURES
//	/*glBindBuffer(GL_ARRAY_BUFFER, buffers[Textures]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(texVertices), texVertices, GL_STATIC_DRAW);
//	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*) 0);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//
//	//BIND COLORS
//	glBindBuffer(GL_ARRAY_BUFFER, buffers[Colors]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
//	glColorPointer(3, GL_FLOAT, 0, (GLvoid*) 0);
//	glEnableClientState(GL_COLOR_ARRAY);*/
//
//	//BIND INDEXES
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[Indexes]);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//
//	glBindVertexArray(0);
//
//}
//
//Sphere::~Sphere()
//{
//}
//
//void Sphere::display() {
//	glBindVertexArray(VAO);
//	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
//	glBindVertexArray(0);
//}