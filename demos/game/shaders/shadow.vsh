//shadow.vsh
#version 120

uniform mat4 mvp;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 uv;

varying vec2 texcoords;

void main(void) {
	gl_Position = mvp * vec4(vertex, 1.0);
	texcoords = uv;
}