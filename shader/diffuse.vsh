//diffuse.vsh
#version 120
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 uv;

varying vec3 fragmentNormal;
varying vec3 worldSpacePosition;
varying vec2 texcoord;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
	worldSpacePosition = (modelMatrix * vec4(vertex, 1.0)).xyz;
	fragmentNormal = normalize(normalMatrix * normal);
	texcoord = uv;
}
