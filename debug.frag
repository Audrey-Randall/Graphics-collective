varying vec4 pos;

void main()
{
  vec4 color = gl_Color;
  vec4 fogColor = vec4(0,0,0,1);
  float dEnd = 1.; //depth at which fog is complete in world coordinates
  float dStart = 0.0; //depth at which fog begins, at the water's surface
  float f = (dEnd + pos.y)/(dEnd - dStart);
  color = f*1.5*color + (1-f*1.5)*fogColor;
  gl_FragColor = color;
}
