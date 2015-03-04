//font.fsh
uniform sampler2D texture;

void main(void) 
{
	vec4 tex = texture2D(texture, gl_TexCoord[0].xy);
	if(tex.r < 1 && tex.g < 1 && tex.b < 1)
	{
		discard;
	}
	
	gl_FragColor = tex * gl_Color;
}