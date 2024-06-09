#version 450 core

layout(location = 0) in vec3 aPos;      // the position variable has attribute position 0
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aNormal;   
layout(location = 4) in vec3 aTangent; 
layout(location = 5) in vec3 aBiTangent; 

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    FragPos = vec3(model * vec4(aPos, 1.0f));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoord = aTexCoord;
}