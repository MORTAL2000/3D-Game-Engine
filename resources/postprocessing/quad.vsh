//quad.vsh
#version 120
attribute vec3 vertex;
uniform mat4 mvp;
varying vec2 texcoords;

void main() {
	gl_Position = mvp * vec4(vertex.xy, 0, 1);
	texcoords = vertex.xy;
}
