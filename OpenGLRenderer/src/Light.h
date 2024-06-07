#pragma once

#include <vector>

#include "Utilities.h"
#include "glad/glad.h"
#include "Transform.h"

const unsigned int MAXNUM_DIRLIGHT = 5, MAXNUM_SPOTLIGHT = 10, MAXNUM_POINTLIGHT = 10; 

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
	void SetLightColor(const glm::vec3& col) { color = col; }

	inline const glm::vec3& GetLightPosition() const { return transform.GetPosition(); }
	inline const glm::vec3& GetLightDirection() const { return transform.GetForwardVector(); }
	inline const glm::vec3& GetLightColor() const { return color; }

	virtual const glm::mat4& BuildProjection() = 0;
	virtual const glm::mat4& BuildView() = 0;
protected:
	LightType type;
	Transform transform;

	glm::vec3 color;

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

	float innerCutOff;
	float outerCutOff;

	float constant = 1.0f;
    float linear;
    float quadratic;
};

class PointLight : public Light
{
public:
	const glm::mat4& BuildProjection() override;
	const glm::mat4& BuildView() override;

	float constant = 1.0f;
	float linear;
	float quadratic;
};

class LightsManager
{
public:
	LightsManager();
	~LightsManager();

	void Init();
	void Update();

	void DrawLightDepthMaps();

	void AddDirectionalLight(glm::vec3 lightRot, glm::vec3 lightColor);
	void AddSpotLight(glm::vec3 lightPos, glm::vec3 lightRot, glm::vec3 lightColor, float innerCutOff, float outerCutOff, float linear, float quadratic);
	void AddPointLight(glm::vec3 lightPos, glm::vec3 lightColor, float linear, float quadratic);

private:
	void BuildLightsFrameBuffer();
	void BuildLightsUniformBuffer();

	GLuint GenerateDepthMap();
	GLuint GenerateDepthCubeMap();
	GLuint GenerateLightUninformBuffer(LightType type, unsigned int size);

	std::vector<DirLight*> dirLights;
	std::vector<SpotLight*> spotLights;
	std::vector<PointLight*> pointLights;

	std::vector<GLuint> FBOsLightsDepth;
	std::vector<GLuint> depthMapsLights;

	GLuint lightsUBOs[LIGHT_Max];
	DirLightData dirLightsData[MAXNUM_DIRLIGHT];
	SpotLightData spotLightsData[MAXNUM_SPOTLIGHT];
	PointLightData pointLightsData[MAXNUM_POINTLIGHT];
};