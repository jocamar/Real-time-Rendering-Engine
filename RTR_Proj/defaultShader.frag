#version 330 core

struct Material {
	sampler2D diffuse_texture1;
	sampler2D diffuse_texture2;
	sampler2D diffuse_texture3;
	sampler2D specular_texture1;
	sampler2D specular_texture2;
	vec3 ambientI;
	vec3 diffuseI;
	vec3 specularI;
    float shininess;
	float opacity;
	int shading_model;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 10

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 brightColor;

uniform vec3 viewPos;
uniform int activeLights;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // == ======================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == ======================================
    // Phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // Phase 2: Point lights
    for(int i = 0; i < activeLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // Phase 3: Spot light
    // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
	vec4 textureColor = mix(texture(material.diffuse_texture1, TexCoords), texture(material.diffuse_texture2, TexCoords), 1);
	
	float opacity = textureColor.a*material.opacity;
	if (opacity < 0.1)
		discard;

	if(material.shading_model == 0)
	{
		vec3 Kd = (1 - textureColor.a)* material.diffuseI + textureColor.a*vec3(textureColor);
		color = vec4(Kd, opacity);

		float brightness = dot(Kd, vec3(0.2126, 0.7152, 0.0722));
		if (brightness > 1.0)
			brightColor = vec4(Kd, 1);
		else brightColor = vec4(0, 0, 0, 1);
	}
	else {
		color = vec4(result, opacity);
		float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
		if (brightness > 1.0)
			brightColor = vec4(result, opacity);
		else brightColor = vec4(0, 0, 0, 1);
	}
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Combine results
	//(1-texture alpha) * material ambient + texture alpha * texture value
	vec4 color = mix(texture(material.diffuse_texture1, TexCoords), texture(material.diffuse_texture2, TexCoords), 1);
	vec4 spec_color = mix(texture(material.specular_texture1, TexCoords), texture(material.specular_texture1, TexCoords), 1);

	vec3 Kd = (1 - color.a)* material.diffuseI + color.a*vec3(color);
	vec3 Ka = (1 - color.a)* material.ambientI + color.a*vec3(color);
	vec3 Ks = (1 - spec_color.a)* material.specularI + spec_color.a*vec3(spec_color);

    vec3 ambient = light.ambient * Ka;
    vec3 diffuse = light.diffuse * diff * Kd;
    vec3 specular = light.specular * spec * Ks;

	if(material.shading_model == 2)
		return ambient + diffuse + specular;
	else 
		return ambient + diffuse;
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
	vec4 color = mix(texture(material.diffuse_texture1, TexCoords), texture(material.diffuse_texture2, TexCoords), 1);
	vec4 spec_color = mix(texture(material.specular_texture1, TexCoords), texture(material.specular_texture1, TexCoords),1);

	vec3 Kd = (1 - color.a)* material.diffuseI + color.a*vec3(color);
	vec3 Ka = (1 - color.a)* material.ambientI + color.a*vec3(color);
	vec3 Ks = (1 - spec_color.a)* material.specularI + spec_color.a*vec3(spec_color);

	vec3 ambient = light.ambient * Ka;
	vec3 diffuse = light.diffuse * diff * Kd;
	vec3 specular = light.specular * spec * Ks;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

	if (material.shading_model == 2)
		return ambient + diffuse + specular;
	else
		return ambient + diffuse;
}