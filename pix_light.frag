//Source: http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/directional-light-per-pixel/

//These varyings have to have the same names as the ones in the vertex shader. If they do, they'll have the interpolated values.
//Uniform means same across all pixels, varying means varies per pixel

varying vec4 diffuse, ambient;
varying vec3 normal, halfVector, viewVector;
uniform sampler2D tex;
uniform sampler2D normal_tex;
uniform int frame;
varying vec2 texCoords;
varying vec3 depth;

void main()
{
    vec3 n,halfV,lightDir, viewDir;
    float NdotL,NdotHV;
    vec4 fogColor = vec4(1,0,0,1);//vec4(0, 0.0862, 0.0549, 1.);

    //lightDir = vec3(gl_LightSource[0].position);
    lightDir = vec3(gl_LightSource[0].position.xyz - viewVector);
    viewDir = normalize(-viewVector);

    /* The ambient term will always be present */
    vec4 color = ambient;
    color.a = 0.1;

    /* a fragment shader can't write a varying variable, hence we need
    a new variable to store the normalized interpolated normal */
    //n = normalize(normal);
    vec2 horizCoords = vec2(texCoords.x, texCoords.y + (1./180. * frame));
    vec2 vertCoords = vec2(texCoords.x + (1./180.*frame), texCoords.y);
    n = normalize(texture2D(normal_tex, texCoords).xyz*normal);
    //n = normalize(((texture2D(normal_tex, horizCoords).xyz + texture2D(normal_tex, vertCoords).xyz)/2)*normal);

    /* compute the dot product between normal and ldir */
    NdotL = max(dot(n,lightDir),0.0);
    //If you just use the dot product the surface disappears when it's negative cuz the normal is backward?
    //No light appears when NdotL is negative on either side of the surface

    //This time, we compute the diffuse component if NdotL is > 0.0. Why diffuse, not specular like before?
    //Why if NdotL > 0? When a dot prod is 0, the vectors are perpendicular
    if (NdotL > 0.0) {
        color += diffuse * NdotL;
        //halfV = normalize(halfVector);
        halfV = normalize(lightDir + viewDir);
        NdotHV = max(dot(n,halfV),0.0);
        color += gl_FrontMaterial.specular *
                gl_LightSource[0].specular *
                pow(NdotHV, gl_FrontMaterial.shininess);
    }
    //The range of light intensities is super high, need to up the ambient and decrease what?
    vec4 texture = texture2D(tex, texCoords).xyzw;
    texture.w = 0.1;
    if(depth.y < 0) {
      float distBelow = -1*depth.y;
      float dEnd = 1.5; //depth at which fog is complete in world coordinates
      float dStart = 0.0; //depth at which fog begins, at the water's surface
      float f = (dEnd - distBelow)/(dEnd - dStart);
      texture = f*texture + (1-f)*fogColor;
      gl_FragColor = fogColor;
    }
    gl_FragColor = color*texture;
    //gl_FragColor = color;
}
