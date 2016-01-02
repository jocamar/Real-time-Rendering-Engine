#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{
	const float gamma = 1.0;
	vec3 hdrColor = texture(scene, TexCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
	//float n = 0.1;                                // the near plane
	//float f = 10.0;                               // the far plane
	//float c = (2.0 * n) / (f + n - hdrColor * (f - n));  // convert to linear values 
	if (bloom)
		hdrColor += bloomColor; // additive blending
								// tone mapping
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

	// also gamma correct while we're at it 
	result = pow(result, vec3(1.0 / gamma));
	FragColor = vec4(result, 1.0f);
}