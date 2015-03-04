//specular.fsh

uniform vec3 cameraPosition;
uniform vec4 color;

varying vec3 fragmentNormal;
varying vec3 worldSpacePosition;
varying vec2 texcoord;

void main(void)
{
	vec3 l = vec3(0, 1, 1);
	gl_FragColor = color * max(dot(fragmentNormal, l), 0.0);
}
