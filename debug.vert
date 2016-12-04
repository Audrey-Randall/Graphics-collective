

void main()
{
	gl_FrontColor = gl_Color;
  vec4 v = ftransform();
	v.x = v.x + sin(v.y*100)*0.2;
	gl_Position = ftransform();
}
