uniform sampler2D fb_tex;
uniform int time;
varying vec2 texCoords;
varying vec4 diffuse, ambient;
varying vec3 normal, halfVector, viewVector;

void main()
{
  vec4 color = gl_Color;
  vec4 fogColor = vec4(0,0,0,1);

  vec2 newCoords = vec2(texCoords.x+sin(time), texCoords.y);
  vec4 texture = texture2D(fb_tex, newCoords).xyzw;

  /*float dEnd = 1.; //depth at which fog is complete in world coordinates
  float dStart = 0.0; //depth at which fog begins, at the water's surface
  float f = (dEnd + pos.y)/(dEnd - dStart);
  texture = f*1.5*texture + (1-f*1.5)*fogColor;*/

  gl_FragColor = texture;
}
