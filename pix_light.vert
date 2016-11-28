//Source: http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/directional-light-per-pixel/

varying vec4 diffuse, ambient;
varying vec3 normal, halfVector, viewVector;
varying vec2 texCoords;
varying vec3 depth;
void main()
{
    /* first transform the normals set by the program into view space and
    normalize the result. Doesn't affect textured normals? */
    normal = normalize(gl_NormalMatrix * gl_Normal);

    /* pass the halfVector to the fragment shader
    halfVector = gl_LightSource[0].halfVector.xyz;
    viewVector = vec3(gl_ModelViewMatrix * gl_Vertex);

    depth = vec3(gl_ModelMatrix*gl_Vertex);

    /* Compute the diffuse, ambient and globalAmbient terms */
    diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
    ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;
    gl_Position = ftransform();
    texCoords = gl_MultiTexCoord0.xy;

}
