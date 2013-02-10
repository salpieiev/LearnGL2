const char *MapFragmentShader = STRINGIFY
(

varying highp vec4 DestinationColor;
varying mediump vec2 TextureCoordOut;

uniform sampler2D Sampler;


highp vec2 SphereMap(in highp vec3 position, in highp vec3 normal);
highp vec3 CubeMap(in highp vec3 position, in highp vec3 normal);


void main()
{
    gl_FragColor = texture2D(Sampler, TextureCoordOut) * DestinationColor;
}


highp vec2 SphereMap(in highp vec3 position, in highp vec3 normal)
{
    highp vec3 reflection = reflect(position, normal);
    highp float m = 2.0 * sqrt(reflection.x * reflection.x + reflection.y * reflection.y + (reflection.z + 1.0) * (reflection.z + 1.0));
    return vec2((reflection.x / m + 0.5), (reflection.y / m + 0.5));
}

highp vec3 CubeMap(in highp vec3 position, in highp vec3 normal)
{
    return reflect(position, normal);
}


);