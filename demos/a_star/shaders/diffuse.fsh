#version 120
uniform sampler2D texture;
uniform int textured;

uniform vec3 cameraPosition;
uniform vec4 color;

varying vec3 fragmentNormal;
varying vec3 worldSpacePosition;
varying vec2 texcoord;

float halfLambert(vec3 v1, vec3 v2)
{
	return dot(v1, v2) * 0.5 + 0.5;
}

void main(void)
{
	vec4 albedo = color;
	if(textured > 0)
	{
		albedo *= texture2D(texture, texcoord);
	}

	vec3 l = vec3(-0.5, 1, 0.5);
	vec4 final = albedo * halfLambert(fragmentNormal, l);
	gl_FragColor = final;
}