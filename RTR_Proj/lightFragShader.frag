
#version 330 core
layout(location = 0) out vec4 color;
layout(location = 1) out vec4 brightColor;

uniform vec3 diffuse;

void main()
{
    color = vec4(diffuse,1.0); // Set all 4 vector values to 1.0f
	brightColor = vec4(diffuse, 1.0);
}