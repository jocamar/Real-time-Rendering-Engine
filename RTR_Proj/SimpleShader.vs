
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 modelview;

void main()
{
	gl_Position = modelview * vec4(position, 1.0f);
	TexCoords = texCoords;
}