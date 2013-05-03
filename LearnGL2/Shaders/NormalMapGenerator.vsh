const char *NormalMapGeneratorVertexShader = STRINGIFY
(

attribute vec3 a_normal;
attribute vec3 a_tangent;
attribute vec2 a_texCoord;

uniform mat4 u_projection;

varying vec3 v_objectSpaceNormal;
varying vec3 v_objectSpaceTangent;
varying vec2 v_texCoord;

const float Distance = 10.0;
const vec2 Offset = vec2(0.5, 0.5);
const vec2 Scale = vec2(8.0, 16.0);

void main()
{
    v_objectSpaceNormal = a_normal;
    v_objectSpaceTangent = a_tangent;
    
    vec4 v = vec4(a_texCoord - Offset, -Distance, 1);
    gl_Position = u_projection * v;
    gl_Position.xy *= Scale;
    
    v_texCoord = a_texCoord;
}

);