#version 330 core

out vec4 FragColor;
in vec3 localPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main()
{
    vec3 normal = normalize(localPos);
    vec3 irrandiance = vec3(0.0f);

    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = normalize(cross(up, normal));
    up = normalize(cross(normal, right));

    float samplerDelta = 0.025f;
    float nrSamples = 0.0f;
    for (float phi = 0.0f; phi < 2.0f * PI; phi += samplerDelta)
    {
        for (float theta = 0.0f; theta < 0.5f * PI; theta += samplerDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 samplerVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            irrandiance += texture(environmentMap, samplerVec).rgb * cos(theta) * sin(theta);
            ++nrSamples;
        }
    }
    irrandiance = PI * irrandiance * (1.0f / float(nrSamples));

    FragColor = vec4(irrandiance, 1.0f);
}