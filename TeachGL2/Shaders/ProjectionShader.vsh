const char *ProjectionVertexShader = STRINGIFY
(

 attribute vec4 Position;
 attribute vec4 SourceColor;
 
 uniform mat4 Projection;
 uniform mat4 Modelview;

 varying vec4 DestinationColor;
 
 void main()
 {
     DestinationColor = SourceColor;
     gl_Position = /*Projection * Modelview * */Position;
 }

);