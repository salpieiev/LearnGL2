const char *MapFragmentShader = STRINGIFY
(

varying mediump vec3 ReflectDir;

uniform samplerCube Sampler;


void main()
{
    gl_FragColor = textureCube(Sampler, ReflectDir);
}

);