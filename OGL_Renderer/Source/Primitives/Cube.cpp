#include "Primitives/Cube.h"
#include "Scene Graph/SceneManager.h"
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

		//Setup vertexes
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

		// Calculate tangent and bitangent for bump mapping
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

		//Add vertexes to list
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
	}

	//Setup bounding box
	this->boundingBox.points[0] = glm::vec3(-0.5, -0.5, -0.5);
	this->boundingBox.points[1] = glm::vec3(0.5, 0.5, 0.5);
	this->boundingBox.center = glm::vec3(0, 0, 0);

	//Generate Array Object and Buffer Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	//Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Send vertex data to buffer
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex),
		&this->vertices[0], GL_STATIC_DRAW);

	//Setup Array Object Attributes

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
	//Vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Tangent));
	//Vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(GLvoid*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}


//@TODO delete VAOs and VBOs?
Cube::~Cube()
{
}



void Cube::display(glm::mat4 transf, int material, Camera *camera, bool shadowMap, Globals::LIGHT_TYPE shadowType) {
	//If there is a material attached to this cube use it, otherwise use material inherited from parent
	int materialToUse;
	if (this->material >= 0)
		materialToUse = this->material;
	else
		materialToUse = material;

	//Fetch the material from the manager
	auto s = this->manager->getMaterial(material);


	// Create the modelview matrix from model matrix and the camera
	glm::mat4 mvm;
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = camera->GetProjectionMatrix();
	mvm = projection * view * transf;

	//If normal rendering pass model and modelview matrixes to shader
	if (!shadowMap)
	{
		glUniformMatrix4fv(s->getShader()->ModelViewLoc, 1, GL_FALSE, glm::value_ptr(mvm));
		glUniformMatrix4fv(s->getShader()->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));
	}
	else
	{
		//If rendering for shadow maps pass appropriate parameters
		if (shadowType == Globals::DIRECTIONAL) {
			glUniformMatrix4fv(manager->getShadowShader()->ModelViewLoc, 1, GL_FALSE, glm::value_ptr(mvm));
		}
		else if (shadowType == Globals::POINT)
		{
			//Since we're using the geometry shader to render all faces in one pass we need to send the viewprojection matrices for the 6 sides
			for (GLuint i = 0; i < 6; ++i)
				glUniformMatrix4fv(glGetUniformLocation(manager->getOmniShadowShader()->Program, ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(camera->cubeViewProjectionMatrixes[i]));
			
			glUniform1f(glGetUniformLocation(manager->getOmniShadowShader()->Program, "far_plane"), 45.0f);
			glUniform3fv(glGetUniformLocation(manager->getOmniShadowShader()->Program, "lightPos"), 1, &(camera->getPosition()[0]));
			//Send just the model matrix, so that the modelview matrixes can be computed in the shader
			glUniformMatrix4fv(manager->getOmniShadowShader()->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));
		}
	}

	//Bind and draw
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}