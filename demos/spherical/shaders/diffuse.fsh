//diffuse.fsh
#version 120
uniform vec3 cameraPosition;
uniform vec3 direction;
uniform vec4 color;

varying vec3 fragmentNormal;
varying vec3 worldSpacePosition;
varying vec2 texcoord;

uniform sampler2D texture;
uniform int textured;

void main(void)
{
	vec4 final = vec4(color.rgb, 1.0);
	if(textured > 0)
	{
		final *= texture2D(texture, texcoord);
	}
	
	gl_FragColor = final * max(dot(fragmentNormal, direction), 0.2);
}
