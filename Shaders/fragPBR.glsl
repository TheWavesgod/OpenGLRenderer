#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

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

struct Material
{
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
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

uniform vec3 camPos;

uniform Material material;

const float PI = 3.14159265359;

// Functions Declaration
vec3 CalcSpotLightRadiance(SpotLight light);
vec3 CalcPointLightRadiance(PointLight light);

float DistributionGGX(vec3 N, vec3 H, float a);
float GeometrySchlickGGX(float NdotV, float k);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

vec3 CalcOutputColor(vec3 radiance, vec3 N, vec3 V, vec3 L, vec3 F0, vec3 albedo, float metallic, float roughness);

void main()
{
    // Get the fragment material settings
    vec3 albedo = texture(material.albedoMap, TexCoord).rgb;
    //vec3 normal = getNormalFromNormalMap(); // TODO: Calculate Normal from normal map
    float metallic = texture(material.metallicMap, TexCoord).r;
    float roughness = texture(material.roughnessMap, TexCoord).r;
    float ao = texture(material.aoMap, TexCoord).r;

    // Get the genaric variables
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - FragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < dirLightsNum; ++i)
    {
        vec3 L = normalize(-dirLights[i].direction);
        result += CalcOutputColor(dirLights[i].color, N, V, L, F0, albedo, metallic, roughness);
    }
    for (int i = 0; i < spotLightsNum; ++i)
    {
        vec3 radiance = CalcSpotLightRadiance(spotLights[i]);
        vec3 L = normalize(spotLights[i].position - FragPos);
        result += CalcOutputColor(radiance, N, V, L, F0, albedo, metallic, roughness);
    }
    for (int i = 0; i < pointLightsNum; ++i)
    {
        vec3 radiance = CalcPointLightRadiance(pointLights[i]);
        vec3 L = normalize(pointLights[i].position - FragPos);   
        result += CalcOutputColor(radiance, N, V, L, F0, albedo, metallic, roughness);
    }
    vec3 ambient = vec3(0.03f) * albedo * ao;
    result += ambient;

    FragColor = vec4(result, 1.0f);
}

vec3 CalcSpotLightRadiance(SpotLight light)
{
    // Calculate intensity 
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    vec3 radiance = light.color * intensity * attenuation;
    return radiance;
}

vec3 CalcPointLightRadiance(PointLight light)
{
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 radiance = light.color * attenuation;
    return radiance;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;

    float nom = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

vec3 CalcOutputColor(vec3 radiance, vec3 N, vec3 V, vec3 L, vec3 F0, vec3 albedo, float metallic, float roughness)
{
    vec3 H = normalize(V + L);

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0f), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0f * max(dot(N, V), 0.0) * max(dot(N, L), 0.0)  + 0.0001;
    vec3 specular = numerator / denominator;     

    vec3 kS = F;
    vec3 kD = vec3(1.0f) - kS;
    kD *= 1.0f - metallic;

    float NdotL = max(dot(N, L), 0.0); 
    vec3 OutputColor = (kD * albedo / PI + specular) * radiance * NdotL;

    return OutputColor;
}