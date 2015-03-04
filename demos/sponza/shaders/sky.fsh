#version 120
uniform vec3 cameraPosition;

varying vec3 fragmentNormal;
varying vec3 worldSpacePosition;
varying vec2 texcoord;

void main(void)
{
	float factor = normalize(worldSpacePosition - cameraPosition).y;
	vec4 c0 = vec4(0.172, 0.290, 0.486, 0.100)*1.25;
	vec4 c1 = vec4(1.0);

	gl_FragColor = vec4(mix(c1, c0, factor).rgb, 1.0);
}
