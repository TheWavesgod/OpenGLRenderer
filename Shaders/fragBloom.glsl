#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D image;

void main()
{
    vec4 color = texture(image, TexCoord);
    
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0f)
    {
        FragColor = vec4(color.rgb, 1.0f);
    }
    else
    {
        FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
}