#version 120
uniform mat4 projectionMatrix;
uniform vec2 scale;
uniform vec2 offset;

attribute vec3 vertex;
varying vec2 texcoord;

void main(void)
{
	gl_Position = projectionMatrix * vec4((vertex.xy * scale) + offset, 0, 1.0);
	texcoord = vertex.xy;
}