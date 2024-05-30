#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

uniform bool gammaCorrection;

uniform float exposure;

void main()
{
    const float gamma = 2.2f;
    vec3 hdrColor = texture(screenTexture, TexCoord).rgb;

    // Reinhard tone mapping
    vec3 mapped = vec3(1.0f) - exp(-hdrColor * exposure);

    FragColor = vec4(mapped, 1.0f);

    // Apply Gamma Correction
    if(gammaCorrection)
    {   
        FragColor.rgb = pow(FragColor.rgb, vec3(1.0f / gamma));
    }
}