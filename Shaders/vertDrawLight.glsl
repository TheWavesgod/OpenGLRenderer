#version 450 core

layout (location = 0) in vec3 aPos;

out vec3 lightColor;

layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
    vec3 camPos;
};

 uniform mat4 models[10];
 uniform vec3 colors[10]; 

void main()
{
    gl_Position = projection * view * models[gl_InstanceID] * vec4(aPos, 1.0f);
    lightColor = colors[gl_InstanceID];
}