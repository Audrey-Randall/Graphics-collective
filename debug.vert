varying vec4 pos;

void main()
{
	gl_FrontColor = gl_Color;
	pos = ftransform();
	gl_Position = ftransform();
}
