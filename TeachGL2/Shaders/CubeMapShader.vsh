const char *MapVertexShader = STRINGIFY
(

attribute vec4 Position;
attribute vec3 Normal;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform vec3 EyePosition;

varying vec3 ReflectDir;


void main()
{
    gl_Position = Projection * Modelview * Position;
    
    // Compute eye direction in object space
    mediump vec3 EyeDir = normalize(Position.xyz - EyePosition);
    
    vec3 reflection = NormalMatrix * reflect(EyeDir, Normal);
    ReflectDir = vec3(reflection.x, -reflection.y, reflection.z);       // Invert top and bottom
    
//    ReflectDir = NormalMatrix * Position.xyz;
}


);