#version 450 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    TexCoord = aPos;
    mat4 cubeview = mat4(mat3(view));
    vec4 pos = projection * cubeview * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}