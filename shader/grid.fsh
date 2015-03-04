//grid.fsh
varying vec3 worldSpacePosition;

void main(void)
{
	vec4 color = vec4(0.6, 0.6, 0.6, 1.0);

	if(worldSpacePosition.z == 0)
	{
		color = vec4(0.8, 0.2, 0.2, 1.0);
	}
	
	if(worldSpacePosition.x == 0)
	{
		color = vec4(0.2, 0.7, 0.2, 1.0);
	}
	
	gl_FragColor = color;
}