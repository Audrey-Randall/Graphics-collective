
void main()
{
  vec4 color = gl_Color;
  color.a = 0.0;
  gl_FragColor = color;
}
