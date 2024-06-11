#version 450 core

layout(location = 0) in vec3 aPos;      // the position variable has attribute position 0
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;   
layout(location = 4) in vec3 aTangent; 
layout(location = 5) in vec3 aBiTangent; 

out vec3 FragPos;
out vec2 TexCoord;
out vec3 viewPos;
out mat3 TBN;

layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
    vec3 camPos;
};
uniform mat4 model;

void main()
{
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0f)));
    vec3 B = normalize(vec3(model * vec4(aBiTangent, 0.0f)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0f)));

    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    FragPos = vec3(model * vec4(aPos, 1.0f)); 
    TexCoord = aTexCoord;
    viewPos = camPos;
    TBN = mat3(T, B, N);
}