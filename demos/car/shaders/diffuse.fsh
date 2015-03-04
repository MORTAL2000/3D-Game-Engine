#version 120
uniform sampler2D texture;
uniform int textured;
uniform sampler2D normalMap;
uniform int hasNormalMap;

uniform vec3 cameraPosition;
uniform vec4 color;

varying vec3 fragmentNormal;
varying vec3 worldSpacePosition;
varying vec2 texcoord;
varying mat3 tbn;

void main(void)
{
	vec4 albedo = color;
	vec3 n = fragmentNormal;

	if(textured > 0)
	{
		albedo *= texture2D(texture, texcoord);
	}

	if(hasNormalMap > 0)
    {
        n = normalize(texture2D(normalMap, texcoord).rgb * 2.0 - 1.0);
        n = normalize(n * tbn);
    }

	vec3 l = vec3(0, 1, 0.2);
	vec4 final = albedo * max(dot(n, l), 0.3);

	gl_FragColor = final;
}
