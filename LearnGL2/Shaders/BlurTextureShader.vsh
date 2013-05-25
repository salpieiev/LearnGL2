const char *BlurTextureVertexShader = STRINGIFY
(

 attribute vec4 a_position;
 attribute vec2 a_texCoord;
 
 uniform mat4 u_projection;
 uniform mat4 u_modelview;
 
 varying vec2 v_texCoord;
 
 void main()
 {
     gl_Position = u_projection * u_modelview * a_position;
     v_texCoord = a_texCoord;
 }

);