#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 viewPos;
in vec3 Normal;
in vec3 Tangent;
in vec3 BiTangent;

// Struct define
struct DirLight
{
    vec3 direction;
    float padding1;
    vec3 color;
    float padding2;

    mat4 lightSpaceMat;
};

struct SpotLight
{
    vec3 position;
    float padding1;
    vec3 direction;
    float padding2;
    vec3 color;

    float innerCutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;

    mat4 lightSpaceMat;
};

struct PointLight 
{
    vec3 position;
    float padding1;
    vec3 color;
    float padding2;

    float constant;
    float linear;
    float quadratic;
    float padding3;

    mat4 lightSpaceMat;
};

// Uniform Variables
layout(std140, binding = 1) uniform DirLightBuffer
{
    DirLight dirLights[5];
    int dirLightsNum;
};

layout(std140, binding = 2) uniform SpotLightBuffer
{
    SpotLight spotLights[5];
    int spotLightsNum;
};

layout(std140, binding = 3) uniform PointLightBuffer
{
    PointLight pointLights[5];
    int pointLightsNum;
};

uniform sampler2D dirDepth[5];
uniform sampler2D spotDepth[5];
uniform samplerCube pointDepth[5];

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;  

uniform vec3 baseColor;
uniform float roughness;


float CalcShadowIn2D(mat4 lightSpaceMat, vec3 lightDir, vec3 normal, sampler2D shadowMap);
float CalcShadowInCube(vec3 lightPos, vec3 viewPos, samplerCube shadowMap);

void main()
{
    vec3 normal = normalize(normalMatrix * Normal);

    vec3 albedo = baseColor;
    float metallic = 0.0f;
    float roughness = roughness;
    float ao = 1.0f;

    vec3 N = normalize(normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < dirLightsNum; ++i)
    {
        vec3 L = normalize(-dirLights[i].direction);
        float shadow = CalcShadowIn2D(dirLights[i].lightSpaceMat, L, normal, dirDepth[i]);
        result += CalcOutputColor(dirLights[i].color, N, V, L, F0, albedo, metallic, roughness) * ao * (1.0f - shadow);
    }
    for (int i = 0; i < spotLightsNum; ++i)
    {
        vec3 L = normalize(spotLights[i].position - FragPos);
        float shadow = CalcShadowIn2D(spotLights[i].lightSpaceMat, L, normal, spotDepth[i]);
        vec3 radiance = CalcSpotLightRadiance(spotLights[i]);
        result += CalcOutputColor(radiance, N, V, L, F0, albedo, metallic, roughness) * ao * (1.0f - shadow);
    }
    for (int i = 0; i < pointLightsNum; ++i)
    {
        vec3 L = normalize(pointLights[i].position - FragPos);  
        float shadow = CalcShadowInCube(pointLights[i].position, viewPos, pointDepth[i]);
        vec3 radiance = CalcPointLightRadiance(pointLights[i]);5
        result += CalcOutputColor(radiance, N, V, L, F0, albedo, metallic, roughness) * ao * (1.0f - shadow);
    }
}

float CalcShadowIn2D(mat4 lightSpaceMat, vec3 lightDir, vec3 normal, sampler2D shadowMap)
{
    vec4 fragPosLightSpace = lightSpaceMat * vec4(FragPos, 1.0f);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5; 
    float currentDepth = projCoords.z;  

    if(currentDepth > 1.0f) return 0.0f;

    float shadow = 0.0f;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;
        }
    }
    shadow /= 9.0f;

    return shadow;
}

float CalcShadowInCube(vec3 lightPos, vec3 viewPos, samplerCube shadowMap)
{
    vec3 fragToLight = FragPos - lightPos;
    float currentDepth = length(fragToLight);

    float shadow = 0.0f;
    float bias = 0.15f; 
    int samples = 20;
    float viewDistance = length(viewPos - FragPos);
    float diskRadius = (1.0f + (viewDistance / 50.0f)) / 25.0f;                   // (viewDistance / far_plane)) / 25.0;  
    for(int i = 0; i < samples; ++i)
    {
        //float closestDepth = texture(shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;            
        float closestDepth = 1.0f;
        closestDepth *= 50.0f;   // * far_plane undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0f;
    }
    shadow /= float(samples);

    return shadow;
}