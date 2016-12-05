varying vec4 pos;

void main()
{
	gl_FrontColor = gl_Color;
	pos = gl_Vertex;
	gl_Position = ftransform();
}
