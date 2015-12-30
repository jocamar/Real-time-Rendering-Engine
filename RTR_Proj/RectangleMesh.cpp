#include "RectangleMesh.h"
#include "SceneManager.h"
#include "Camera.h"

RectangleMesh::RectangleMesh(const char *id, SceneManager *manager, int parts, float t_x, float t_y) : Mesh(id)
{
	this->manager = manager;
	this->parts = parts;
	this->material = -1;

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
				-0.5f + curr_x + step,	-0.5f + curr_y + step,	0.0f,  0.0f,  0.0f, -1.0f,  t_x*(curr_x + step),	t_y*(curr_y + step),
				-0.5f+curr_x+step,	-0.5f+curr_y,		0.0f,  0.0f,  0.0f, -1.0f,  t_x*(curr_x+step),	t_y*curr_y,
				-0.5f+curr_x+step,	-0.5f+curr_y+step,	0.0f,  0.0f,  0.0f, -1.0f,  t_x*(curr_x+step),	t_y*(curr_y+step),
				-0.5f + curr_x,		-0.5f + curr_y,		0.0f,  0.0f,  0.0f, -1.0f,  t_x*curr_x,			t_y*curr_y,
				-0.5f+curr_x,		-0.5f+curr_y+step,	0.0f,  0.0f,  0.0f, -1.0f,  t_x*curr_x,			t_y*(curr_y+step)
			};

			vertices.insert(std::end(vertices), std::begin(tmp), std::end(tmp));
		}
	}

	GLfloat* vert_arr = new GLfloat[parts*parts * 6 * 8];
	for (int i = 0; i < vertices.size(); i++)
		vert_arr[i] = vertices[i];

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//BIND VERTICES
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
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



void RectangleMesh::display(glm::mat4 transf, int material, Camera *camera) {
	int materialToUse;
	if (this->material >= 0)
		materialToUse = this->material;
	else
		materialToUse = material;

	auto s = this->manager->getMaterial(material);

	s->use(camera);

	// Create camera transformation
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(camera->Zoom, (float)1280 / (float)720, 0.1f, 1000.0f);

	// Pass the matrices to the shader
	glUniformMatrix4fv(s->getShader()->ViewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(s->getShader()->ProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(s->getShader()->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, parts*parts*6);//, GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);
}