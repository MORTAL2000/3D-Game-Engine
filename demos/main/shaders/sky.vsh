
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 uv;

varying vec3 worldSpacePosition;
varying vec2 texcoords;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1);
	worldSpacePosition = (modelMatrix * vec4(vertex, 1)).xyz;
	texcoords = uv;
}
