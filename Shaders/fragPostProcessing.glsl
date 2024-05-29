#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

uniform bool gammaCorrection;

void main()
{
    FragColor = texture(screenTexture, TexCoord);
    // Apply Gamma Correction
    if(gammaCorrection)
    {   
        float gamma = 2.2f;
        FragColor.rgb = pow(FragColor.rgb, vec3(1.0f / gamma));
    }
}