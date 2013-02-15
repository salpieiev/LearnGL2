const char *VertexSkinningVertexShader = STRINGIFY
(

 const int NUM_MATRICES = 32;

const int c_zero = 0;
const int c_one = 1;
const int c_two = 2;
const int c_three = 3;

// Store 32 4x3 matrices as an array of floats representing each matrix in row-major order i.e. 3 vec4
uniform vec4 matrix_palette[NUM_MATRICES * 3];

attribute vec4 a_position;
attribute vec3 a_normal;
attribute vec4 a_color;

// Matrix veights - 4 entries per vertex
attribute vec4 a_matrixWeights;

// Matrix palette indices
attribute vec4 a_matrixIndices;
 
uniform mat4 u_projection;
uniform mat4 u_modelview;
uniform mat3 u_normalMatrix;
uniform vec3 u_lightPosition;
uniform vec3 u_ambientLight;
uniform vec3 u_specularLight;
uniform float u_shininess;

varying vec4 v_color;


void skin_position(in vec4 position, in float m_wt, in int m_indx, inout vec4 skinned_position);
void skin_normal(in vec3 normal, in float m_wt, in int m_indx, inout vec3 skinned_normal);
void do_skinning(in vec4 position, in vec3 normal, inout vec4 skinned_position, inout vec3 skinned_normal);


void main()
{
    vec4 skinnedPosition;
    vec3 skinnedNormal;
    
    do_skinning(a_position, a_normal, skinnedPosition, skinnedNormal);
    
    gl_Position = u_projection * u_modelview * skinnedPosition;
    
    vec3 N = u_normalMatrix * skinnedNormal;
    vec3 L = u_normalMatrix * u_lightPosition;
    vec3 E = vec3(0, 0, 1);
    vec3 H = normalize(L + E);
    float df = max(0.0, dot(N, L));
    float sf = max(0.0, dot(N, H));
    sf = pow(sf, u_shininess);
    
    vec3 diffuseColor = vec3(a_color.x, a_color.y, a_color.z);
    vec3 color = u_ambientLight + df * diffuseColor + sf * u_specularLight;
    v_color = vec4(color, 1);
}


void skin_position(in vec4 position, in float m_wt, in int m_indx, inout vec4 skinned_position)
{
    vec4 tmp;
    tmp.x = dot(position, matrix_palette[m_indx]);
    tmp.y = dot(position, matrix_palette[m_indx + c_one]);
    tmp.z = dot(position, matrix_palette[m_indx + c_two]);
    tmp.w = position.w;
    
    skinned_position += m_wt * tmp;
}

void skin_normal(in vec3 normal, in float m_wt, in int m_indx, inout vec3 skinned_normal)
{
    vec3 tmp;
    tmp.x = dot(normal, matrix_palette[m_indx].xyz);
    tmp.y = dot(normal, matrix_palette[m_indx + c_one].xyz);
    tmp.z = dot(normal, matrix_palette[m_indx + c_two].xyz);
    
    skinned_normal += m_wt * tmp;
}

void do_skinning(in vec4 position, in vec3 normal, inout vec4 skinned_position, inout vec3 skinned_normal)
{
    skinned_position = vec4(float(c_zero));
    skinned_normal = vec3(float(c_zero));
    
    float m_wt;
    int m_indx;
    
    // Transform position and normal to eye space using matrix
    // palette with four matrices used to transform a vertex
    
    m_wt = a_matrixWeights[0];
    m_indx = int(a_matrixIndices[0]) * c_three;
    skin_position(position, m_wt, m_indx, skinned_position);
    skin_normal(normal, m_wt, m_indx, skinned_normal);
    
    m_wt = a_matrixWeights[1];
    m_indx = int(a_matrixIndices[1]) * c_three;
    skin_position(position, m_wt, m_indx, skinned_position);
    skin_normal(normal, m_wt, m_indx, skinned_normal);
    
    m_wt = a_matrixWeights[2];
    m_indx = int(a_matrixIndices[2]) * c_three;
    skin_position(position, m_wt, m_indx, skinned_position);
    skin_normal(normal, m_wt, m_indx, skinned_normal);
    
    m_wt = a_matrixWeights[3];
    m_indx = int(a_matrixIndices[3]) * c_three;
    skin_position(position, m_wt, m_indx, skinned_position);
    skin_normal(normal, m_wt, m_indx, skinned_normal);
}

);




























