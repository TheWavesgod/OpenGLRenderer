#pragma once

#include "glm.hpp"

struct DirLightData
{
	glm::vec3 direction;
	float padding1;
	glm::vec3 color;
	float padding2;
};

struct SpotLightData
{
	glm::vec3 position;
	float padding1;
	glm::vec3 direction;
	float padding2;
	glm::vec3 color;

	float innerCutOff;
	float outerCutOff;
	
	float constant;
	float linear;
	float quadratic;
};

// TODO: Learn how to align data memory
struct PointLightData
{
	glm::vec3 position;
	float padding1;
	glm::vec3 color;
	float padding2;

	float constant;
	float linear;
	float quadratic;
	float padding3;
};