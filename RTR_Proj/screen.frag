#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float E = 0.02;
float F = 0.30;
float W = 11.2;

vec3 Uncharted2Tonemap(vec3 x)
{
	return ((x*(A*x + C*B) + D*E) / (x*(A*x + B) + D*F)) - E / F;
}

vec3 AlternativeTonemap(vec3 x)
{
	float L = 0.2126 * x.r + 0.7152 * x.r + 0.0722 * x.b;
	float nL = L/(1.0+L);
	float scale = nL / L;
	vec3 result = vec3(x.r * scale, x.g * scale, x.b * scale);
	return result;
}

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(scene, TexCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
	hdrColor += bloomColor; // additive blending
	vec3 result;
	if (bloom)
	{
		//Adapted Reinhard
		//hdrColor *= exposure;
		//result = AlternativeTonemap(hdrColor);

		//Uncharted 2
		hdrColor *= exposure; 
		float ExposureBias = 2.0f;
		result = Uncharted2Tonemap(ExposureBias*hdrColor);
		vec3 whiteScale = Uncharted2Tonemap(vec3(W));
		whiteScale = vec3(1.0 / whiteScale.x, 1.0 / whiteScale.y, 1.0 / whiteScale.z);
		result = result*whiteScale;

		//Reinhard
		//result = hdrColor / (hdrColor + vec3(1.0));// tone mapping
		//result = vec3(1.0) - exp(-result * exposure);
	}
	else result = hdrColor;

	result = pow(result, vec3(1.0 / gamma));

	//gamma correct 
	FragColor = vec4(result, 1.0f);
}