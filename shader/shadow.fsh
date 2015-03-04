//shadow.fsh
#version 120

uniform sampler2D map;
varying vec2 texcoords;

void main(void) {
	float depth = gl_FragCoord.z;

	if(texture2D(map, texcoords).a == 0.0) discard;
	gl_FragColor = vec4(depth, 0.0, 0.0, 0.0);
}
