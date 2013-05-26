const char *BlurTextureFragmentShader = STRINGIFY
(
 
 precision highp float;
 
 uniform sampler2D u_sampler;
 uniform mediump float u_blurStep;

 varying highp vec2 v_texCoord;
 
 void main()
 {
     vec4 sample0 = texture2D(u_sampler, vec2(v_texCoord.x - u_blurStep, v_texCoord.y - u_blurStep));
     vec4 sample1 = texture2D(u_sampler, vec2(v_texCoord.x + u_blurStep, v_texCoord.y - u_blurStep));
     vec4 sample2 = texture2D(u_sampler, vec2(v_texCoord.x + u_blurStep, v_texCoord.y + u_blurStep));
     vec4 sample3 = texture2D(u_sampler, vec2(v_texCoord.x - u_blurStep, v_texCoord.y + u_blurStep));
     
     gl_FragColor = (sample0 + sample1 + sample2 + sample3) / 4.0;
 }

);