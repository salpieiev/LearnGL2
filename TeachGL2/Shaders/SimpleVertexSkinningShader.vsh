const char *VertexSkinningVertexShader = STRINGIFY
(

 const int NUM_MATRICES = 5;
 
 attribute vec4 a_position;
 attribute vec4 a_color;
 attribute vec2 a_boneWeights;
 attribute vec2 a_boneIndices;
 
 uniform mat4 u_projection;
 uniform mat4 u_modelview/*[NUM_MATRICES]*/;
 
 varying vec4 v_color;
 
 void main()
 {
     vec4 p0 = u_modelview[int(a_boneIndices.x)] * a_position;
     vec4 p1 = u_modelview[int(a_boneIndices.y)] * a_position;
     vec4 p = p0 * a_boneWeights.x + p1 * a_boneWeights.y;
     gl_Position = u_projection * u_modelview * a_position;
     
     v_color = a_color;
 }
 
);




























