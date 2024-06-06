#version 450 core

layout(location = 0) in vec3 aPos;      // the position variable has attribute position 0
layout(location = 1) in vec4 aColor;    // the color variable has attribute position 1 
layout(location = 2) in vec2 aTexCoord;

out vec4 vertexColor; // specify a color output to the fragment shader
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
    vertexColor = aColor;
    TexCoord = aTexCoord * 10;
}