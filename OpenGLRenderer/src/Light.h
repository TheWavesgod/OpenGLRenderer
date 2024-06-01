#pragma once

#include <vector>

#include "glad/glad.h"
#include "Transform.h"

enum LightType
{
	LIGHT_Directional = 0,
	LIGHT_Spot,
	LIGHT_Point,

	LIGHT_Max
};

class Light
{
public:
	void SetLightPosition(const glm::vec3& pos) { transform.SetPosition(pos); }
	void SetLightRotation(const glm::vec3& rot) { transform.SetRotation(rot); }

	virtual const glm::mat4& BuildProjection() = 0;
	virtual const glm::mat4& BuildView() = 0;
protected:
	LightType type;
	Transform transform;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	inline const LightType GetType() const { return type; }
};

class DirLight : public Light
{
public:
	const glm::mat4& BuildProjection() override;
	const glm::mat4& BuildView() override;
};

class SpotLight : public Light
{
public:
	const glm::mat4& BuildProjection() override;
	const glm::mat4& BuildView() override;

private:
	float innerCutOff;
	float outerCutOff;

	float constant;
    float linear;
    float quadratic;
};

class PointLight : public Light
{
public:
	const glm::mat4& BuildProjection() override;
	const glm::mat4& BuildView() override;

private:
	float constant;
	float linear;
	float quadratic;

};

class LightsManager
{
public:
	std::vector<Light*> lights;

	void DrawLightDepthMaps();

private:
	void BuildLightsFrameBuffer();
	GLuint GenerateDepthMap();
	GLuint GenerateDepthCubeMap();

	std::vector<GLuint> FBOsLightsDepth;
	std::vector<GLuint> depthMapsLights;
};