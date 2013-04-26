const char *NormalMappingVertexShader = STRINGIFY
(

attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec3 a_tangent;
attribute vec2 a_texCoord;

uniform mat4 u_projection;
uniform mat4 u_modelview;

varying vec2 v_texCoord;
varying vec3 v_objectSpaceNormal;
varying vec3 v_objectSpaceTangent;

void main()
{
    gl_Position = u_projection * u_modelview * a_position;
    
    v_objectSpaceNormal = a_normal;
    v_objectSpaceTangent = a_tangent;
    v_texCoord = a_texCoord;
}

);