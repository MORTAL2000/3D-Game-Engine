//grid.vsh
#version 120
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

attribute vec3 vertex;
varying vec3 worldSpacePosition;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
	worldSpacePosition = vertex.xyz;
}
