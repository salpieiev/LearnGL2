const char *FogVertexShader = STRINGIFY
(

attribute vec4 a_position;
attribute vec4 a_color;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform vec4 u_eyePosition;
uniform vec4 u_clipPlane;

varying vec4 v_color;
varying float v_eyeDist;
varying float v_clipDist;


void main()
{
    vec4 viewPosition = u_modelview * a_position;
    vec4 eyeDistVector = viewPosition - u_eyePosition;
    
    v_eyeDist = sqrt(eyeDistVector.x * eyeDistVector.x + eyeDistVector.y * eyeDistVector.y + eyeDistVector.z * eyeDistVector.z);
    
    gl_Position = u_projection * u_modelview * a_position;
    
    v_color = a_color;
    
    // Compute the distance between the vertex and the clip plane
    v_clipDist = dot(a_position.xyz, u_clipPlane.xyz) + u_clipPlane.w;
}


);