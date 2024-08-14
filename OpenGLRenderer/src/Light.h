#pragma once

#include <vector>

#include "Utilities.h"
#include "glad/glad.h"
#include "Transform.h"

const unsigned int MAXNUM_DIRLIGHT = 5, MAXNUM_SPOTLIGHT = 5, MAXNUM_POINTLIGHT = 5; 

class SceneNode;
class Shader;
class GameObject;

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
	void SetLightIntensity(float val) { intensity = val; }

	inline const glm::vec3& GetLightPosition() const { return transform.GetPosition(); }
	inline const glm::vec3& GetLightDirection() const { return transform.GetForwardVector(); }
	inline const glm::vec3& GetLightRotation() const { return transform.GetRotation(); }
	inline const glm::vec3& GetLightColor() const { return color; }
	inline const Transform& GetLightTransform() const { return transform; }
	inline const float GetLightIntensity() const { return intensity; }

	virtual const glm::mat4& BuildProjection() = 0;
	virtual const glm::mat4& BuildView() = 0;
	virtual const glm::mat4& BuildLightSpaceMatrix() { return BuildProjection() * BuildView(); };
protected:
	LightType type;
	Transform transform;

	glm::vec3 color;
	float intensity = 5.0f;

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

	float innerCutOffInDegree;
	float outerCutOffInDegree;

	float constant = 1.0f;
    float linear;
    float quadratic;
};

class PointLight : public Light
{
public:
	const glm::mat4& BuildProjection() override;
	const glm::mat4& BuildView() override;

	const glm::mat4 BuildShadowMatrix(int i);

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

	void DrawLightDepthMaps(const std::vector<GameObject*> objs);
	void DrawLightCubes();

	void AddDirectionalLight(glm::vec3 lightRot, glm::vec3 lightColor);
	void AddSpotLight(glm::vec3 lightPos, glm::vec3 lightRot, glm::vec3 lightColor, float innerCutOff, float outerCutOff, float linear, float quadratic);
	void AddPointLight(glm::vec3 lightPos, glm::vec3 lightColor, float linear, float quadratic);

	std::vector<DirLight*>& GetDirlights() { return dirLights; }
	std::vector<SpotLight*>& GetSpotlights() { return spotLights; }
	std::vector<PointLight*>& GetPointlights() { return pointLights; }

private:
	void BuildLightsFrameBuffer();
	void BuildLightsUniformBuffer();

	void BindDepthMapSamplerForShader(Shader* shader);

	GLuint GenerateDepthMap();
	GLuint GenerateDepthCubeMap();
	GLuint GenerateLightUninformBuffer(LightType type, unsigned int size);

	void GenerateLightCube();

	std::vector<DirLight*> dirLights;
	std::vector<SpotLight*> spotLights;
	std::vector<PointLight*> pointLights;

	std::vector<GLuint> FBOsLightsDepth;
	std::vector<GLuint> depthMapsLights;

	GLuint lightsUBOs[LIGHT_Max];
	DirLightData dirLightsData[MAXNUM_DIRLIGHT];
	SpotLightData spotLightsData[MAXNUM_SPOTLIGHT];
	PointLightData pointLightsData[MAXNUM_POINTLIGHT];

	GLuint lightCubeVBO;
	GLuint lightCubeVAO;
	GLuint lightCubeEBO;
};