const char *BlendingVertexShader = STRINGIFY
(

attribute vec4 a_position;
attribute vec4 a_color;

uniform mat4 u_modelview;

varying vec4 v_color;

void main()
{
    gl_Position = u_modelview * a_position;
    v_color = a_color;
}

);