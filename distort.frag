uniform sampler2D fb_tex;
uniform int time;
varying vec2 texCoords;
varying vec4 diffuse, ambient;
varying vec3 normal, halfVector, viewVector;

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

void main()
{
  vec4 color = gl_Color;
  vec4 fogColor = vec4(0,0,0,1);
  vec4 origTex = texture2D(fb_tex, texCoords);

  vec2 newCoords = vec2(texCoords.x+0.002*sin(texCoords.y*300+0.3*time)*sin(origTex.x), texCoords.y+0.002*sin(texCoords.x*300+0.5*time));
  vec4 texture = texture2D(fb_tex, texCoords).xyzw;

  /*float dEnd = 1.; //depth at which fog is complete in world coordinates
  float dStart = 0.0; //depth at which fog begins, at the water's surface
  float f = (dEnd + pos.y)/(dEnd - dStart);
  texture = f*1.5*texture + (1-f*1.5)*fogColor;*/

  if(texture.x == 0 || texture.y == 0 || texture.z == 0) gl_FragColor = (0.5,0.5,0.5,0);
  else gl_FragColor = texture;
}
