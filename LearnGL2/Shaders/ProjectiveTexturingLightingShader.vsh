const char *ProjectiveTexturingLightingVertexShader = STRINGIFY
(

attribute vec4 a_position;
attribute vec4 a_color;
attribute vec3 a_normal;

uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform mat4 u_projectorProjection;
uniform mat4 u_projectorModelview;
uniform mat3 u_normalMatrix;
uniform mat3 u_biasMatrix;
uniform vec3 u_lightPosition;
uniform vec3 u_projectorPosition;

varying vec4 v_color;
varying vec3 v_normal;
varying vec3 v_lightDirection;
varying vec3 v_projectorDirection;
varying vec3 v_projectiveTexCoord;


void main()
{
    gl_Position = u_projection * u_modelview * a_position;
    
    // Compute the projector coordinate axes
    vec3 look = -normalize(u_projectorPosition);
    vec3 right = cross(vec3(0.0, 0.0, 1.0), look);
    vec3 up = cross(look, right);
    
    // Create a view matrix for the light
    mat4 projectorModelview = mat4
    (
        right, dot(right, -u_projectorPosition),
        up, dot(up, -u_projectorPosition),
        look, dot(look, -u_projectorPosition),
        0.0, 0.0, 0.0, 1.0
    );
    
    // Bias matrix
    mat3 biasMatrix = mat3
    (
        0.5, 0.0, 0.5,
        0.0, -0.5, 0.5,
        0.0, 0.0, 1.0
    );
    
    v_projectiveTexCoord = (u_projectorProjection * a_position * projectorModelview).xyz * biasMatrix;

    v_color = a_color;
    /*v_normal = u_normalMatrix * a_normal;
    v_lightDirection = u_normalMatrix * normalize(u_lightPosition - a_position.xyz);
    v_projectorDirection = u_normalMatrix * normalize(u_projectorPosition - a_position.xyz);*/
    
    v_normal = a_normal;
    v_lightDirection = normalize(u_lightPosition - a_position.xyz);
    v_projectorDirection = normalize(a_position.xyz - u_projectorPosition);
}


);