#include "Light.h"
#include "Shader.h"
#include "Window.h"
#include "SceneNode.h"

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

LightsManager::LightsManager()
{
}

LightsManager::~LightsManager()
{
}

void LightsManager::Init()
{
	BuildLightsFrameBuffer();
	BuildLightsUniformBuffer();

	BindDepthMapSamplerForShader(Shader::GetShaderByIndex(1));
	BindDepthMapSamplerForShader(Shader::GetShaderByIndex(4));
}

void LightsManager::Update()
{
	int dirLightsNum = dirLights.size();
	for (size_t i = 0; i < dirLights.size(); ++i)
	{
		dirLightsData[i].direction = dirLights[i]->GetLightDirection();
		dirLightsData[i].color = dirLights[i]->GetLightColor();
		dirLightsData[i].lightSpaceMat = dirLights[i]->BuildLightSpaceMatrix();
	}

	int spotLightsNum = spotLights.size();
	for (size_t i = 0; i < spotLights.size(); ++i)
	{
		spotLightsData[i].position = spotLights[i]->GetLightPosition();
		spotLightsData[i].direction = spotLights[i]->GetLightDirection();
		spotLightsData[i].color = spotLights[i]->GetLightColor();
		spotLightsData[i].innerCutOff = spotLights[i]->innerCutOff;
		spotLightsData[i].outerCutOff = spotLights[i]->outerCutOff;
		spotLightsData[i].constant = spotLights[i]->constant;
		spotLightsData[i].linear = spotLights[i]->linear;
		spotLightsData[i].quadratic = spotLights[i]->quadratic;
		spotLightsData[i].lightSpaceMat = spotLights[i]->BuildLightSpaceMatrix();
	}

	int pointLightsNum = pointLights.size();
	for (size_t i = 0; i < pointLights.size(); ++i)
	{
		pointLightsData[i].position = pointLights[i]->GetLightPosition();
		pointLightsData[i].color = pointLights[i]->GetLightColor();
		pointLightsData[i].constant = pointLights[i]->constant;
		pointLightsData[i].linear = pointLights[i]->linear;
		pointLightsData[i].quadratic = pointLights[i]->quadratic;
		pointLightsData[i].lightSpaceMat = pointLights[i]->BuildLightSpaceMatrix();
	}

	glBindBuffer(GL_UNIFORM_BUFFER, lightsUBOs[LIGHT_Directional]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, dirLightsNum * sizeof(DirLightData), dirLightsData);
	glBufferSubData(GL_UNIFORM_BUFFER, MAXNUM_DIRLIGHT * sizeof(DirLightData), sizeof(int), &dirLightsNum);
	glBindBuffer(GL_UNIFORM_BUFFER, lightsUBOs[LIGHT_Spot]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, spotLightsNum * sizeof(SpotLightData), spotLightsData);
	glBufferSubData(GL_UNIFORM_BUFFER, MAXNUM_SPOTLIGHT * sizeof(SpotLightData), sizeof(int), &spotLightsNum);
	glBindBuffer(GL_UNIFORM_BUFFER, lightsUBOs[LIGHT_Point]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, pointLightsNum * sizeof(PointLightData), pointLightsData);
	glBufferSubData(GL_UNIFORM_BUFFER, MAXNUM_POINTLIGHT * sizeof(PointLightData), sizeof(int), &pointLightsNum);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void LightsManager::DrawLightDepthMaps(SceneNode* node)
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	
	Shader* usingShader = Shader::GetShaderByIndex(5);
	usingShader->Use();
	for (size_t i = 0; i < dirLights.size(); ++i)
	{
		usingShader->SetUniformMat4("lightProjection", dirLights[i]->BuildProjection());
		usingShader->SetUniformMat4("lightView", dirLights[i]->BuildView());
		glBindFramebuffer(GL_FRAMEBUFFER, FBOsLightsDepth[i]);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glClear(GL_DEPTH_BUFFER_BIT);	
		node->DrawToLightDepthMap(usingShader);
	}

	for (size_t i = 0; i < spotLights.size(); ++i)
	{
		usingShader->SetUniformMat4("lightProjection", spotLights[i]->BuildProjection());
		usingShader->SetUniformMat4("lightView", spotLights[i]->BuildView());
		glBindFramebuffer(GL_FRAMEBUFFER, FBOsLightsDepth[dirLights.size() + i]);
		glClear(GL_DEPTH_BUFFER_BIT);
		node->DrawToLightDepthMap(usingShader);
	}

	for (size_t i = 0; i < pointLights.size(); ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOsLightsDepth[dirLights.size() + spotLights.size() + i]);
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, window::GetWindowPointer()->GetWidth(), window::GetWindowPointer()->GetHeight());
}

void LightsManager::BuildLightsFrameBuffer()
{
	size_t lightNum = dirLights.size() + spotLights.size() + pointLights.size();
	FBOsLightsDepth.resize(lightNum);
	depthMapsLights.resize(lightNum);

	glGenFramebuffers(lightNum, FBOsLightsDepth.data());
	for (size_t i = 0; i < dirLights.size(); ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOsLightsDepth[i]);
		depthMapsLights[i] = GenerateDepthMap();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapsLights[i], 0);
		glDrawBuffer(GL_NONE);						// Set Framebuffer Draw and Read to GL_NONE can explicitly tell OpenGL we are not going to draw anything
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	for (size_t i = dirLights.size(); i < dirLights.size() + spotLights.size(); ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOsLightsDepth[i]);
		depthMapsLights[i] = GenerateDepthMap();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapsLights[i], 0);
		glDrawBuffer(GL_NONE);						
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	for (size_t i = dirLights.size() + spotLights.size(); i < lightNum; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOsLightsDepth[i]);
		depthMapsLights[i] = GenerateDepthCubeMap();
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapsLights[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Bind the depth map to the Textrue Unit
	for (size_t i = 0; i < dirLights.size() + pointLights.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE10 + i);
		glBindTexture(GL_TEXTURE_2D, depthMapsLights[i]);
	}
}

void LightsManager::BuildLightsUniformBuffer()
{
	lightsUBOs[LIGHT_Directional] = GenerateLightUninformBuffer(LIGHT_Directional, MAXNUM_DIRLIGHT * sizeof(DirLightData) + sizeof(int));
	lightsUBOs[LIGHT_Spot] = GenerateLightUninformBuffer(LIGHT_Spot, MAXNUM_SPOTLIGHT * sizeof(SpotLightData) + sizeof(int));
	lightsUBOs[LIGHT_Point] = GenerateLightUninformBuffer(LIGHT_Point, MAXNUM_POINTLIGHT * sizeof(PointLightData) + sizeof(int));
}

void LightsManager::BindDepthMapSamplerForShader(Shader* shader)
{	
	shader->Use();
	for (size_t i = 0; i < dirLights.size(); ++i)
	{
		shader->SetUniformInt("dirDepth[" + std::to_string(i) + "]", 10 + i);
	}
	for (size_t i = 0; i < spotLights.size(); ++i)
	{
		shader->SetUniformInt("spotDepth[" + std::to_string(i) + "]", 10 + i + dirLights.size());
	}
}

GLuint LightsManager::GenerateDepthMap()
{
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	return depthMap;
}

GLuint LightsManager::GenerateDepthCubeMap()
{
	GLuint depthCubeMap;
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return depthCubeMap;
}

GLuint LightsManager::GenerateLightUninformBuffer(LightType type, unsigned int size)
{
	GLuint ubo;
	int lightNum = 0;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_UNIFORM_BUFFER, size - sizeof(int), sizeof(int), &lightNum);
	glBindBufferBase(GL_UNIFORM_BUFFER, type + 1, ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return ubo;
}


void LightsManager::AddDirectionalLight(glm::vec3 lightRot, glm::vec3 lightColor)
{
	DirLight* newlight = new DirLight();
	newlight->SetLightRotation(lightRot);
	newlight->SetLightColor(lightColor);

	dirLights.push_back(newlight);
}

void LightsManager::AddSpotLight(glm::vec3 lightPos, glm::vec3 lightRot, glm::vec3 lightColor, float innerCutOff, float outerCutOff, float linear, float quadratic)
{
	SpotLight* newlight = new SpotLight();
	newlight->SetLightPosition(lightPos);
	newlight->SetLightRotation(lightRot);
	newlight->SetLightColor(lightColor);
	newlight->innerCutOff = innerCutOff;
	newlight->outerCutOff = outerCutOff;
	newlight->linear = linear;
	newlight->quadratic = quadratic;

	spotLights.push_back(newlight);
}

void LightsManager::AddPointLight(glm::vec3 lightPos, glm::vec3 lightColor, float linear, float quadratic)
{
	PointLight* newlight = new PointLight();
	newlight->SetLightPosition(lightPos);
	newlight->SetLightColor(lightColor);
	newlight->linear = linear;
	newlight->quadratic = quadratic;

	pointLights.push_back(newlight);
}


const glm::mat4& DirLight::BuildProjection()
{
	float near_plane = -200.0f, far_plane = 200.0f;
	return glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
}

const glm::mat4& DirLight::BuildView()
{
	glm::vec3 center = glm::vec3(0.0f);
	glm::vec3 lightPos = center - this->GetLightDirection() * 100.0f;

	return glm::lookAt(lightPos, center, glm::vec3( 0.0f, 1.0f, 0.0f));
}

const glm::mat4& SpotLight::BuildProjection()
{
	float nearPlane = 1.0f;
	float farPlane = 100.0f; 

	return glm::perspective(glm::radians(outerCutOff * 2.0f), 1.0f, nearPlane, farPlane);
}

const glm::mat4& SpotLight::BuildView()
{
	return glm::lookAt(transform.GetPosition(), transform.GetPosition() + transform.GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4& PointLight::BuildProjection()
{
	float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
	float near = 1.0f;
	float far = 25.0f;
	return glm::perspective(glm::radians(90.0f), aspect, near, far);
}

const glm::mat4& PointLight::BuildView()
{
	return glm::mat4();
}
