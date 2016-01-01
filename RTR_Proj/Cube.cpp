#include "Cube.h"
#include "SceneManager.h"
#include "Camera.h"

Cube::Cube(const char *id, SceneManager *manager) : Mesh(id) 
{
	this->manager = manager;
	this->material = -1;

	enum { Vertices, /*Indexes,*/ numVBO};

	GLfloat vertexes[36][8] = {
        // Positions          // Normals           // Texture Coords
		{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f},
		{ 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f},
        { 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f},
        { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f},
		{-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f},
        {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f},
		
		{-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f},
		{-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f},
		{ 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f},
		{ 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f},
		{ 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f},
		{-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f},
		
        {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
		{-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f},
		{-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
        {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f},
		{-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
		
		{ 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f },
        { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
		{ 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{ 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f},
		{ 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f},
		
        {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f},
		{ 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f},
		{ 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f},
        { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f},
		{-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f},
		{-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f},
		
        {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f},
		{ 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f},
        { 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f},
        { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f},
		{-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f},
        {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f}
    };

	for (int i = 0; i < 36; i+=3)
	{
		Vertex v0, v1, v2;

		v0.Position.x = vertexes[i][0];
		v0.Position.y = vertexes[i][1];
		v0.Position.z = vertexes[i][2];
		v0.Normal.x = vertexes[i][3];
		v0.Normal.y = vertexes[i][4];
		v0.Normal.z = vertexes[i][5];
		v0.TexCoords.x = vertexes[i][6];
		v0.TexCoords.y = vertexes[i][7];

		v1.Position.x = vertexes[i+1][0];
		v1.Position.y = vertexes[i+1][1];
		v1.Position.z = vertexes[i+1][2];
		v1.Normal.x = vertexes[i+1][3];
		v1.Normal.y = vertexes[i+1][4];
		v1.Normal.z = vertexes[i+1][5];
		v1.TexCoords.x = vertexes[i+1][6];
		v1.TexCoords.y = vertexes[i+1][7];

		v2.Position.x = vertexes[i+2][0];
		v2.Position.y = vertexes[i+2][1];
		v2.Position.z = vertexes[i+2][2];
		v2.Normal.x = vertexes[i+2][3];
		v2.Normal.y = vertexes[i+2][4];
		v2.Normal.z = vertexes[i+2][5];
		v2.TexCoords.x = vertexes[i+2][6];
		v2.TexCoords.y = vertexes[i+2][7];

		glm::vec3 Edge1 = v1.Position - v0.Position;
		glm::vec3 Edge2 = v2.Position - v0.Position;

		float DeltaU1 = v1.TexCoords.x - v0.TexCoords.x;
		float DeltaV1 = v1.TexCoords.y - v0.TexCoords.y;
		float DeltaU2 = v2.TexCoords.x - v0.TexCoords.x;
		float DeltaV2 = v2.TexCoords.y - v0.TexCoords.y;

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		glm::vec3 Tangent, Bitangent;

		Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
		Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
		Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

		Bitangent.x = f * (-DeltaU2 * Edge1.x - DeltaU1 * Edge2.x);
		Bitangent.y = f * (-DeltaU2 * Edge1.y - DeltaU1 * Edge2.y);
		Bitangent.z = f * (-DeltaU2 * Edge1.z - DeltaU1 * Edge2.z);

		v0.Tangent = -Tangent;
		v1.Tangent = -Tangent;
		v2.Tangent = -Tangent;

		v0.Bitangent = -Bitangent;
		v1.Bitangent = -Bitangent;
		v2.Bitangent = -Bitangent;

		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//BIND VERTICES
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, TexCoords));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}



Cube::~Cube()
{
}



void Cube::display(glm::mat4 transf, int material, Camera *camera) {
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
	glDrawArrays(GL_TRIANGLES, 0, 36);//, GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);

	s->unUse(camera);
}