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

struct Material
{
    sampler2D albedo;
    sampler2D normal;
    sampler2D height;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D ao;
    sampler2D emissive;
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

uniform Material material;

uniform sampler2D dirDepth[5];
uniform sampler2D spotDepth[5];
uniform samplerCube pointDepth[5];

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;  

uniform bool useHeight;
uniform float heightScale;
uniform bool useEmissive;
uniform float emissiveScale;
uniform mat4 model;

const float PI = 3.14159265359;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
     vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
); 

// Functions Declaration
mat3 CalculateTBN();
vec2 ParallaxMapping(mat3 TBN);
vec3 CalNormalFromMap(vec2 texCoord, mat3 TBN);

vec3 CalcSpotLightRadiance(SpotLight light);
vec3 CalcPointLightRadiance(PointLight light);
float CalcShadowIn2D(mat4 lightSpaceMat, vec3 lightDir, vec3 normal, sampler2D shadowMap);
float CalcShadowInCube(vec3 lightPos, vec3 viewPos, samplerCube shadowMap);

float DistributionGGX(vec3 N, vec3 H, float a);
float GeometrySchlickGGX(float NdotV, float k);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

vec3 CalcOutputColor(vec3 radiance, vec3 N, vec3 V, vec3 L, vec3 F0, vec3 albedo, float metallic, float roughness);

void main()
{
    mat3 TBN = CalculateTBN();
    vec2 texCoord = useHeight ? ParallaxMapping(TBN) : TexCoord;
    // if(texCoord.x > 1.0f || texCoord.y > 1.0f || texCoord.x < 0.0f || texCoord.y < 0.0f)
    //     discard;

    // Get the fragment material settings
    vec3 albedo = texture(material.albedo, texCoord).rgb;
    vec3 normal = CalNormalFromMap(texCoord, TBN);
    float metallic = texture(material.metallic, texCoord).r;
    float roughness = texture(material.roughness, texCoord).r;
    float ao = texture(material.ao, texCoord).r;

    // Get the genaric variables
    vec3 N = normalize(normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

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
        vec3 radiance = CalcPointLightRadiance(pointLights[i]);
        result += CalcOutputColor(radiance, N, V, L, F0, albedo, metallic, roughness) * ao * (1.0f - shadow);
    }

    //Emissive
    vec3 emissiveLight = useEmissive ? emissiveScale * texture(material.emissive, texCoord).rgb : vec3(0.0f);
    result += emissiveLight;

    vec3 R = reflect(-V, N);
    vec3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0f), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0f - kS;
    kD *= 1.0 - metallic;
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0f;
    vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 envBRDF  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    result += ambient;
    FragColor = vec4(result, 1.0f);
}

mat3 CalculateTBN()
{
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * Tangent);
    vec3 B = normalize(normalMatrix * BiTangent);
    vec3 N = normalize(normalMatrix * Normal);

    mat3 TBN = mat3(T, B, N);

    return TBN;    
}

vec2 ParallaxMapping(mat3 TBN)
{
    mat3 inverseTBN = transpose(TBN);

    vec3 tangentViewPos = inverseTBN * viewPos;
    vec3 tangentFragPos = inverseTBN * FragPos;
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoord = P / numLayers;
    // Get initial values
    vec2 currentTexCoord = TexCoord;
    float currentDepthMapValue = 1.0f - texture(material.height, TexCoord).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoord -= deltaTexCoord;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = 1.0f - texture(material.height, currentTexCoord).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoord = currentTexCoord + deltaTexCoord;

    // get depth after and before collision for linear interpolation
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = 1.0f - texture(material.height, prevTexCoord).r - currentLayerDepth + layerDepth;
    // Interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoord = prevTexCoord * weight + currentTexCoord * (1.0 - weight);
    return finalTexCoord;
}

vec3 CalNormalFromMap(vec2 texCoord, mat3 TBN)
{
    vec3 normal = texture(material.normal, texCoord).rgb;
    normal = normal * 2.0f - 1.0f;
    normal = normalize(TBN * normal);
    return normal;
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

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
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