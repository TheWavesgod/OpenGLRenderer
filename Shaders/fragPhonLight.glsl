#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct DirLight
{
    vec3 direction;
    float padding1;
    vec3 color;
    float padding2;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

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
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

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
};

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


layout(std140, binding = 1) uniform DirLightBuffer
{
    DirLight dirLights[5];
    int dirLightsNum;
};

layout(std140, binding = 2) uniform SpotLightBuffer
{
    SpotLight spotLights[10];
    int spotLightsNum;
};

layout(std140, binding = 3) uniform PointLightBuffer
{
    PointLight pointLights[10];
    int pointLightsNum;
};


uniform samplerCube skybox;
vec3 CalcSkybox(vec3 normal, vec3 viewDir);

// TODO: Set Uniform value bool blinn;
bool blinn = true;
float CalSpec(vec3 lightDir, vec3 normal, vec3 viewDir);

void main()
{
   // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < dirLightsNum; ++i)
    {
        result += CalcDirLight(dirLights[i], norm, viewDir);       // phase 1: Directional lighting
    }
    for (int i = 0; i < spotLightsNum; ++i)
    {
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);    // phase 2: Spot light
    }
    for (int i = 0; i < pointLightsNum; ++i)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  // phase 3: Point lights
    }

    FragColor = vec4(result, 1.0);
}

float CalSpec(vec3 lightDir, vec3 normal, vec3 viewDir)
{
    float spec = 0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(halfwayDir, normal), 0.0), material.shininess * 4.0f);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    return spec;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    float spec = CalSpec(lightDir, normal, viewDir);
    vec3 ambient  = 0.1f * light.color * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.color * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.color * spec * vec3(texture(material.specular, TexCoord));
    return (ambient + diffuse + specular); 
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    // specular shading
    float spec = CalSpec(lightDir, normal, viewDir);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = 0.1f * light.color * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.color * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.color * spec * vec3(texture(material.specular, TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Calculate intensity 
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
    // specular shading
    float spec = CalSpec(lightDir, normal, viewDir);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = 0.1f * light.color * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.color * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.color * spec * vec3(texture(material.specular, TexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    diffuse  *= intensity;
    specular *= intensity;
    return (ambient + diffuse + specular);
}

// TODO: Set Ambient reflection
vec3 CalcSkybox(vec3 normal, vec3 viewDir)
{
    vec3 I = -viewDir;
    vec3 R = reflect(I, normal);

    vec3 color = texture(skybox, R).rgb;

    return color;
}