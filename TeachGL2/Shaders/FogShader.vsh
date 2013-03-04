const char *FogVertexShader = STRINGIFY
(

attribute vec4 a_position;
attribute vec4 a_color;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform vec4 u_eyePosition;

varying vec4 v_color;
varying float v_eyeDist;


void main()
{
    vec4 viewPosition = u_modelview * a_position;
    vec4 eyeDistVector = viewPosition - u_eyePosition;
    
    v_eyeDist = sqrt(eyeDistVector.x * eyeDistVector.x + eyeDistVector.y * eyeDistVector.y + eyeDistVector.z * eyeDistVector.z);
    
    gl_Position = u_projection * u_modelview * a_position;
    
    v_color = a_color;
}


);