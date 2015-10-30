#include "Mesh.h"

GLuint Mesh::getVAO() {
	return VAO;
}

void Mesh::display() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}