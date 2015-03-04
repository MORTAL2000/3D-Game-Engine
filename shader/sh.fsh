//spherical harmonics
varying vec3 diffuse_color;

void main(void)
{
	gl_FragColor = vec4(diffuse_color, 1);
}
