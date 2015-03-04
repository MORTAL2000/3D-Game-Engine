#version 120
uniform mat4 projectionMatrix;

attribute vec3 vertex;
attribute vec2 uv;

varying vec2 texcoord;

void main(void)
{
	gl_Position = projectionMatrix * vec4(vertex.xy, 0, 1.0);
	texcoord = uv;
}