#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(scene, TexCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
	vec3 result;
	if (bloom)
	{
		hdrColor += bloomColor; // additive blending

		result = hdrColor / (hdrColor + vec3(1.0));// tone mapping
		result = vec3(1.0) - exp(-result * exposure);
		result = pow(result, vec3(1.0 / gamma));
	}
	else result = hdrColor;

	//gamma correct 
	FragColor = vec4(result, 1.0f);
}