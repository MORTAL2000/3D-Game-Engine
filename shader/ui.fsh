#version 120
uniform sampler2D texture;
uniform vec4 color;
varying vec2 texcoord;

uniform int colorOnly;

void main(void)
{
	vec4 final = color;
	if(colorOnly == 0)
	{
		final *= texture2D(texture, texcoord).rgba;
	}

	gl_FragColor = final;
}