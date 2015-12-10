//base.fsh
#version 120
uniform sampler2D screen;
varying vec2 texcoords;

void main() {
	gl_FragColor = texture2D(screen, texcoords);
}