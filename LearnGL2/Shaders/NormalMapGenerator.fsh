const char *NormalMapGeneratorFragmentShader = STRINGIFY
(

varying mediump vec3 v_objectSpaceNormal;
varying mediump vec3 v_objectSpaceTangent;
varying mediump vec2 v_texCoord;

uniform sampler2D u_sampler;

void main()
{
    // Extract the perturbed normal from the texture
    highp vec3 tangentSpaceNormal = texture2D(u_sampler, v_texCoord).zxy * 2.0 - 1.0;
    
    // Create a set of basis vectors
    highp vec3 n = normalize(v_objectSpaceNormal);
    highp vec3 t = normalize(v_objectSpaceTangent);
    highp vec3 b = normalize(cross(n, t));
    
    // Change the perturbed normal from tangent space to object space
    highp mat3 basis = mat3(n, t, b);
    highp vec3 N = basis * tangentSpaceNormal;
    
    // Transform the normal from unit space to color space
    gl_FragColor = vec4((N + 1.0) * 0.5, 1);
}

);