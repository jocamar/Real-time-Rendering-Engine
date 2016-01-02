#include "RectangleMesh.h"
#include "SceneManager.h"
#include "Camera.h"

RectangleMesh::RectangleMesh(const char *id, SceneManager *manager, int parts, float t_x, float t_y) : Mesh(id)
{
	this->manager = manager;
	this->parts = parts;
	this->material = -1;

	float step = 1.0 / static_cast<float>(parts);

	for (int i = 0; i < parts; i++)
	{
		float curr_x = i*step;
		for (int j = 0; j < parts; j++)
		{
			float curr_y = j*step;
			GLfloat tmp[6][8] = {
				{-0.5f+curr_x,			-0.5f+curr_y,			0.0f,  0.0f,  0.0f, -1.0f,  t_x*curr_x,			t_y*curr_y			},
				{-0.5f + curr_x + step,	-0.5f + curr_y + step,	0.0f,  0.0f,  0.0f, -1.0f,  t_x*(curr_x+step),	t_y*(curr_y+step)	},
				{-0.5f+curr_x+step,		-0.5f+curr_y,			0.0f,  0.0f,  0.0f, -1.0f,  t_x*(curr_x+step),	t_y*curr_y		},
				{-0.5f+curr_x+step,		-0.5f+curr_y+step,		0.0f,  0.0f,  0.0f, -1.0f,  t_x*(curr_x+step),	t_y*(curr_y+step)	},
				{-0.5f + curr_x,		-0.5f + curr_y,			0.0f,  0.0f,  0.0f, -1.0f,  t_x*curr_x,			t_y*curr_y		},
				{-0.5f+curr_x,			-0.5f+curr_y+step,		0.0f,  0.0f,  0.0f, -1.0f,  t_x*curr_x,			t_y*(curr_y+step)	}
			};

			for (int w = 0; w < 6; w+=3)
			{
				Vertex v0, v1, v2;

				v0.Position.x = tmp[w][0];
				v0.Position.y = tmp[w][1];
				v0.Position.z = tmp[w][2];
				v0.Normal.x = tmp[w][3];
				v0.Normal.y = tmp[w][4];
				v0.Normal.z = tmp[w][5];
				v0.TexCoords.x = tmp[w][6];
				v0.TexCoords.y = tmp[w][7];

				v1.Position.x = tmp[w + 1][0];
				v1.Position.y = tmp[w + 1][1];
				v1.Position.z = tmp[w + 1][2];
				v1.Normal.x = tmp[w + 1][3];
				v1.Normal.y = tmp[w + 1][4];
				v1.Normal.z = tmp[w + 1][5];
				v1.TexCoords.x = tmp[w + 1][6];
				v1.TexCoords.y = tmp[w + 1][7];

				v2.Position.x = tmp[w + 2][0];
				v2.Position.y = tmp[w + 2][1];
				v2.Position.z = tmp[w + 2][2];
				v2.Normal.x = tmp[w + 2][3];
				v2.Normal.y = tmp[w + 2][4];
				v2.Normal.z = tmp[w + 2][5];
				v2.TexCoords.x = tmp[w + 2][6];
				v2.TexCoords.y = tmp[w + 2][7];

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
			
		}
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



RectangleMesh::~RectangleMesh()
{

}



void RectangleMesh::display(glm::mat4 transf, int material, Camera *camera, bool shadowMap) {
	int materialToUse;
	if (this->material >= 0)
		materialToUse = this->material;
	else
		materialToUse = material;

	auto s = this->manager->getMaterial(material);

	s->use(camera, shadowMap);

	// Create camera transformation
	glm::mat4 mvm;
	if (!shadowMap)
	{
		glm::mat4 view;
		view = camera->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(camera->Zoom, (float)1280 / (float)720, 0.1f, 1000.0f);
		mvm = projection * view * transf;
	}
	else
	{
		mvm = camera->ViewProjMatrix * transf;
	}
	// Pass the matrices to the shader
	if(!shadowMap)
	{
		glUniformMatrix4fv(s->getShader()->ModelViewLoc, 1, GL_FALSE, glm::value_ptr(mvm));
		glUniformMatrix4fv(s->getShader()->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));
	}
	else
	{
		glUniformMatrix4fv(manager->getShadowShader()->ModelViewLoc, 1, GL_FALSE, glm::value_ptr(mvm));
	}

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, parts*parts*6);//, GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);

	s->unUse(camera);
}