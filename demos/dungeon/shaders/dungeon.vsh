//dungeon.vsh
#version 120
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform int hasNormalMap;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec2 uv;

varying vec3 fragmentNormal;
varying vec3 worldSpacePosition;
varying vec2 texcoord;
varying mat3 tbn;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.0);
	worldSpacePosition = (modelMatrix * vec4(vertex, 1.0)).xyz;
	fragmentNormal = normalize(normalMatrix * normal);
	texcoord = uv;

	if(hasNormalMap > 0)
	{
		vec3 n = fragmentNormal;
		vec3 c1 = cross(n, vec3(0, 0, 1));
		vec3 c2 = cross(n, vec3(0, 1, 0));
		vec3 tangent = (length(c1) > length(c2)) ? c1 : c2;
		vec3 t = normalize(normalMatrix * tangent);
		vec3 b = cross(tangent, n);
		tbn = transpose(mat3(t, b, n));
	}
}
