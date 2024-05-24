#pragma once

#include "Transform.h"

class Light
{
public:
	void SetLightPosition(const glm::vec3& pos) { transform.SetPosition(pos); }
	void SetLightRotation(const glm::vec3& rot) { transform.SetRotation(rot); }

protected:
	Transform transform;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

private:
		
};

class DirLight : public Light
{
public:


};

class PointLight : public Light
{
public:

private:
	float constant;
	float linear;
	float quadratic;

};

class SpotLight : public Light
{
public:


private:
	float innerCutOff;
	float outerCutOff;

	float constant;
    float linear;
    float quadratic;
};

class LightsManager
{
public:

};