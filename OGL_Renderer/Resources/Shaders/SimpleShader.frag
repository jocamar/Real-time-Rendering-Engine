#version 330 core

struct Material {
	int diffuse_texture1_active;
	int diffuse_texture2_active;
	int specular_texture1_active;
	int specular_texture2_active;
	int normal_texture1_active;
	sampler2D diffuse_texture1;
	sampler2D diffuse_texture2;
	sampler2D diffuse_texture3;
	sampler2D specular_texture1;
	sampler2D specular_texture2;
	sampler2D normal_texture1;
	vec3 ambientI;
	vec3 diffuseI;
	vec3 specularI;
	float shininess;
	float opacity;
	int shading_model;
};

in vec2 TexCoords;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 brightColor;

uniform Material material;

void main()
{
	vec4 textureColor;
	if (material.diffuse_texture1_active == 1 && material.diffuse_texture2_active == 1)
		textureColor = mix(texture(material.diffuse_texture1, TexCoords), texture(material.diffuse_texture2, TexCoords), 1);
	else textureColor = texture(material.diffuse_texture1, TexCoords);

	float opacity = textureColor.a*material.opacity;

	vec3 Kd;
	if (material.diffuse_texture1_active == 1)
		Kd = (1 - textureColor.a)* material.diffuseI + textureColor.a*vec3(textureColor);
	else Kd = material.diffuseI;

	color = vec4(Kd, opacity);

	float brightness = dot(Kd, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0)
		brightColor = vec4(Kd, 1);
	else brightColor = vec4(0, 0, 0, 1);
}