varying vec4 diffuse, ambient;
varying vec3 normal, halfVector, viewVector;
varying vec2 texCoords;

void main()
{
    vec4 pos = gl_Vertex;
    pos.z = 0.2; //I guess we must be in window space, clipping plane is at 1
    gl_Position = pos;
    texCoords = gl_MultiTexCoord0.xy;

}
