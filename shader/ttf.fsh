#version 120
uniform sampler2D texture;
uniform vec4 color;
varying vec2 texcoord;

void main(void)
{
	gl_FragColor = vec4(color.rgb, texture2D(texture, texcoord).r * color.a);
}