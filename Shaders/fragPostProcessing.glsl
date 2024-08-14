#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform sampler2D bloomBlur;

uniform bool gammaCorrection;
uniform bool useBloom;

uniform float exposure;

void main()
{
    const float gamma = 2.2f;
    vec3 hdrColor = texture(screenTexture, TexCoord).rgb;
    if(useBloom)
    {
        vec3 bloomColor = texture(bloomBlur, TexCoord).rgb;
        // Additive blending with bloom effect
        hdrColor += bloomColor;    
    }

    // Reinhard tone mapping
    vec3 mapped = vec3(1.0f) - exp(-hdrColor * exposure);

    FragColor = vec4(mapped, 1.0f);

    // Apply Gamma Correction
    if(gammaCorrection)
    {   
        FragColor.rgb = pow(FragColor.rgb, vec3(1.0f / gamma));
    }
}