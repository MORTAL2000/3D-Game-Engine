//shadow.fsh
#version 120

uniform sampler2D map;
varying vec2 texcoords;

void main(void) {
//	if(texture2D(map, texcoords).a == 0.0) discard;
	gl_FragColor = vec4(gl_FragCoord.z, 0.0, 0.0, 0.0);
}
