const char *NormalMappingFragmentShader = STRINGIFY
(

uniform sampler2D u_sampler;

uniform highp vec3 u_ambientMaterial;
uniform highp vec3 u_diffuseMaterial;
uniform highp vec3 u_specularMaterial;
uniform highp vec3 u_lightVector;
uniform highp vec3 u_eyeVector;
uniform highp float u_shininess;

varying mediump vec2 v_texCoord;
varying mediump vec3 v_objectSpaceNormal;
varying mediump vec3 v_objectSpaceTangent;

void main()
{
    // Extract the perturbed normal from the texture
    highp vec3 tangentSpaceNormal = texture2D(u_sampler, v_texCoord).yxz * 2.0 - 1.0;
    
    // Create a set of basis vectors
    highp vec3 n = normalize(v_objectSpaceNormal);
    highp vec3 t = normalize(v_objectSpaceTangent);
    highp vec3 b = normalize(cross(n, t));
    
    // Change the perturbed normal from tangent space to object space
    highp mat3 basis = mat3(n, t, b);
    highp vec3 N = basis * tangentSpaceNormal;
    
    // Perform lighting math
    highp vec3 L = normalize(u_lightVector);
    highp vec3 E = normalize(u_eyeVector);
    highp vec3 H = normalize(L + E);
    highp float df = max(0.0, dot(N, L));
    highp float sf = max(0.0, dot(N, H));
    sf = pow(sf, u_shininess);
    
    lowp vec3 color = u_ambientMaterial + df * u_diffuseMaterial + sf * u_specularMaterial;
    gl_FragColor = vec4(color, 1);
}

);