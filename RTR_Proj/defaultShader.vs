
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	mat4 lightSpaceMatrix;
	sampler2D shadowMap;
};

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 Tangent;
out vec3 Bitangent;

#define NR_POINT_LIGHTS 10
out vec4 FragPosDirectionalLightSpace;
out vec4 FragPosPointLightSpace[NR_POINT_LIGHTS];

uniform mat4 model;
uniform mat4 modelview;
uniform DirLight dirLight;

void main()
{
    gl_Position = modelview * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = normalize(mat3(transpose(inverse(model))) * normal);  
    TexCoords = texCoords;
	Tangent = normalize(vec3(model * vec4(tangent, 0.0)));
	Bitangent = normalize(vec3(model * vec4(bitangent, 0.0)));
	FragPosDirectionalLightSpace = dirLight.lightSpaceMatrix * vec4(FragPos, 1.0);
} 