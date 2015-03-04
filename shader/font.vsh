//font.vsh
uniform float scale;

void main(void)
{
	vec4 vertex = gl_Vertex;
	vertex.xy *= scale;
   	gl_Position = gl_ModelViewProjectionMatrix * vertex;

   	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
} 
