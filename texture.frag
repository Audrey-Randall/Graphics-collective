
varying vec4 diffuse, ambient;
varying vec3 normal, halfVector;
uniform sampler2D tex;
varying vec2 texCoords;

void main()
{
    vec3 n,halfV,lightDir, viewDir;
    float NdotL,NdotHV;

    lightDir = vec3(gl_LightSource[0].position);

    /* The ambient term will always be present */
    vec4 color = ambient;

    n = normalize(normal);

    /* compute the dot product between normal and ldir */
    NdotL = max(dot(n,lightDir),0.0);

    if (NdotL > 0.0) {
        color += diffuse * NdotL;
        halfV = normalize(halfVector);
        NdotHV = max(dot(n,halfV),0.0);
        color += gl_FrontMaterial.specular *
                gl_LightSource[0].specular *
                pow(NdotHV, gl_FrontMaterial.shininess);
    }
    vec4 texture = texture2D(tex, texCoords).xyzw;
    gl_FragColor = texture;

}
