#include "RectangleMesh.h"

RectangleMesh::RectangleMesh(std::string id, int parts, float t_x, float t_y)
{
	this->idMesh = id;
	this->parts = parts;

	enum { Vertices, /*Indexes,*/ numVBO };

	vector<GLfloat> vertices;
	float step = 1.0 / static_cast<float>(parts);

	for (int i = 0; i < parts; i++)
	{
		float curr_x = i*step;
		for (int j = 0; j < parts; j++)
		{
			float curr_y = j*step;
			GLfloat tmp[] = {
				-0.5f+curr_x,		-0.5f+curr_y,		0.0f,  0.0f,  0.0f, -1.0f,  t_x*curr_x,			t_y*curr_y,
				-0.5f+curr_x+step,	-0.5f+curr_y,		0.0f,  0.0f,  0.0f, -1.0f,  t_x*(curr_x+step),	t_y*curr_y,
				-0.5f+curr_x+step,	-0.5f+curr_y+step,	0.0f,  0.0f,  0.0f, -1.0f,  t_x*(curr_x+step),	t_y*(curr_y+step),
				-0.5f+curr_x+step,	-0.5f+curr_y+step,	0.0f,  0.0f,  0.0f, -1.0f,  t_x*(curr_x+step),	t_y*(curr_y+step),
				-0.5f+curr_x,		-0.5f+curr_y+step,	0.0f,  0.0f,  0.0f, -1.0f,  t_x*curr_x,			t_y*(curr_y+step),
				-0.5f+curr_x,		-0.5f+curr_y,		0.0f,  0.0f,  0.0f, -1.0f,  t_x*curr_x,			t_y*curr_y
			};

			vertices.insert(std::end(vertices), std::begin(tmp), std::end(tmp));
		}
	}

	GLfloat* vert_arr = new GLfloat[parts*parts * 6 * 8];
	for (int i = 0; i < vertices.size(); i++)
		vert_arr[i] = vertices[i];

	GLuint buffers[numVBO];
	glGenVertexArrays(1, &VAO);
	glGenBuffers(numVBO, buffers);

	glBindVertexArray(VAO);

	//BIND VERTICES
	glBindBuffer(GL_ARRAY_BUFFER, buffers[Vertices]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), vert_arr, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

RectangleMesh::~RectangleMesh()
{

}

void RectangleMesh::display() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, parts*parts*6);//, GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);
}