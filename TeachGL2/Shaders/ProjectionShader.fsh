const char *ProjectionFragmentShader = STRINGIFY
(

 uniform highp vec4 DestinationColor;
 
 void main()
 {
     gl_FragColor = DestinationColor;
 }

);