//matcap.vsh

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 uv;

varying vec3 e;
varying vec3 n;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
	n = normalize(normalMatrix * normal);
	e = normalize(vec3(viewMatrix * modelMatrix * vec4(vertex, 1.0)));
}
