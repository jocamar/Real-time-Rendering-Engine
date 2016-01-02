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

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	mat4 lightSpaceMatrix;
	sampler2D shadowMap;
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
in vec3 Tangent;
in vec3 Bitangent;
in vec4 FragPosDirectionalLightSpace;
in vec4 FragPosPointLightSpace[NR_POINT_LIGHTS];

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 brightColor;

uniform vec3 viewPos;
uniform int activeLights;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 color, vec4 spec_color);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 color, vec4 spec_color);
vec3 CalcBumpedNormal();
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, sampler2D shadowMap);

void main()
{    
    // Properties
    vec3 norm;

	if (material.normal_texture1_active == 1)
		norm = CalcBumpedNormal();
	else norm = Normal;

    vec3 viewDir = normalize(viewPos - FragPos);

	vec4 tex_color;
	if (material.diffuse_texture1_active == 1 && material.diffuse_texture2_active == 1)
		tex_color = mix(texture(material.diffuse_texture1, TexCoords), texture(material.diffuse_texture2, TexCoords), 1);
	else if (material.diffuse_texture1_active == 1)
		tex_color = texture(material.diffuse_texture1, TexCoords);
	else tex_color = vec4(0, 0, 0, 0);

	vec4 spec_color;
	if (material.specular_texture1_active == 1 && material.specular_texture2_active == 1)
		spec_color = mix(texture(material.specular_texture1, TexCoords), texture(material.specular_texture1, TexCoords), 1);
	else if (material.specular_texture1_active == 1)
		spec_color = texture(material.specular_texture1, TexCoords);
	else spec_color = vec4(0, 0, 0, 0);
    
    // == ======================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == ======================================
    // Phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir, tex_color, spec_color);
    // Phase 2: Point lights
    for(int i = 0; i < activeLights; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, tex_color, spec_color);    
    // Phase 3: Spot light
    // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
	vec4 textureColor; 
	if(material.diffuse_texture1_active == 1 && material.diffuse_texture2_active == 1)
		textureColor = mix(texture(material.diffuse_texture1, TexCoords), texture(material.diffuse_texture2, TexCoords), 1);
	else textureColor = texture(material.diffuse_texture1, TexCoords);
	
	float opacity = textureColor.a*material.opacity;
	if (opacity < 0.1)
		discard;

	if(material.shading_model == 0)
	{
		vec3 Kd;
		if(material.diffuse_texture1_active == 1)
			Kd = (1 - textureColor.a)* material.diffuseI + textureColor.a*vec3(textureColor);
		else Kd = material.diffuseI;

		color = vec4(Kd, opacity);

		float brightness = dot(Kd, vec3(0.2126, 0.7152, 0.0722));
		if (brightness > 1.0)
			brightColor = vec4(Kd, 1);
		else brightColor = vec4(0, 0, 0, 1);
	}
	else {
		color = vec4(result, opacity);
		//color = texture(dirLight.shadowMap, TexCoords);

		float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
		if (brightness > 1.0)
			brightColor = vec4(result, opacity);

		else brightColor = vec4(0, 0, 0, 1);
	}
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 color, vec4 spec_color)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Combine results
	float shadow = ShadowCalculation(FragPosDirectionalLightSpace, lightDir, dirLight.shadowMap);

	vec3 Kd;
	vec3 Ka;
	if (material.diffuse_texture1_active == 1)
	{
		Kd = (1 - color.a)* material.diffuseI + color.a*vec3(color);
		Ka = (1 - color.a)* material.ambientI + color.a*vec3(color);
	}
	else
	{
		Kd = material.diffuseI;
		Ka = material.ambientI;
	}

	vec3 Ks;
	if (material.specular_texture1_active == 1)
		Ks = (1 - spec_color.a)* material.specularI + spec_color.a*vec3(spec_color);
	else
		Ks = material.specularI;

    vec3 ambient = light.ambient * Ka;
    vec3 diffuse = light.diffuse * diff * Kd;
    vec3 specular = light.specular * spec * Ks;

	if(material.shading_model == 2)
		return ambient + (1.0 - shadow) * (diffuse + specular);
	else 
		return ambient + (1.0 - shadow) * diffuse;
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 color, vec4 spec_color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results

	vec3 Kd;
	vec3 Ka;
	if (material.diffuse_texture1_active == 1)
	{
		Kd = (1 - color.a)* material.diffuseI + color.a*vec3(color);
		Ka = (1 - color.a)* material.ambientI + color.a*vec3(color);
	}
	else
	{
		Kd = material.diffuseI;
		Ka = material.ambientI;
	}

	vec3 Ks;
	if (material.specular_texture1_active == 1)
		Ks = (1 - spec_color.a)* material.specularI + spec_color.a*vec3(spec_color);
	else
		Ks = material.specularI;

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

vec3 CalcBumpedNormal()
{
	vec3 tangent0 = normalize(Tangent - dot(Tangent, Normal) * Normal);
	vec3 BumpMapNormal = texture(material.normal_texture1, TexCoords).xyz;
	BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
	vec3 NewNormal;
	mat3 TBN = mat3(Tangent, -Bitangent, Normal);
	NewNormal = TBN * BumpMapNormal;
	NewNormal = normalize(NewNormal);
	return NewNormal;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, sampler2D shadowMap)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// Transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	// Get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	// Check whether current frag pos is in shadow
	float bias = max(0.001 * (1.0 - dot(Normal, lightDir)), 0.0005);
	//float bias = 0.0;
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth + bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	return shadow;
}