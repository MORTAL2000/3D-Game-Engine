// arrow.fsh
varying vec3 worldSpacePosition;
varying vec3 fragmentNormal;

void main(void)
{
	float diffuse = max(dot(fragmentNormal, vec3(0, 1, 1)), 0.1);
	vec4 color = vec4(0.6, 0.6, 0.6, 1.0) * diffuse;
	
	if(worldSpacePosition.x > 1)
	{
		color = vec4(0.9, 0.1, 0.1, 1.0);
	}
	
	if(worldSpacePosition.z < -1)
	{
		color = vec4(0.1, 0.9, 0.1, 1.0);
	}
	
	if(worldSpacePosition.y > 1)
	{
		color = vec4(0.1, 0.1, 0.9, 1.0);
	}
	
	gl_FragColor = color;
}