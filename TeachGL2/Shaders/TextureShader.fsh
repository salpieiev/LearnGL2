const char *TextureFragmentShader = STRINGIFY
(

 varying highp vec4 DestinationColor;
 
 void main()
 {
     gl_FragColor = DestinationColor;
 }

);