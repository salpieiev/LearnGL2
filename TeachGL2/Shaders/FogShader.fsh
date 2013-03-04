const char *FogFragmentShader = STRINGIFY
(


precision mediump float;

uniform vec4 u_fogColor;
uniform float u_fogMaxDist;
uniform float u_fogMinDist;

varying vec4 v_color;
varying float v_eyeDist;


void main()
{
    float fogFactor = (u_fogMaxDist - v_eyeDist) / (u_fogMaxDist - u_fogMinDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    
    vec4 fogColor = /*fogFactor * */u_fogColor;
    
    gl_FragColor = v_color * fogFactor + fogColor * (1.0 - fogFactor);
}


);