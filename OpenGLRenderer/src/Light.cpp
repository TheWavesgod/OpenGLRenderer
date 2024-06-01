#include "Light.h"
#include "Window.h"

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

void LightsManager::DrawLightDepthMaps()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	
	for (size_t i = 0; i < lights.size(); ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOsLightsDepth[i]);
		glClear(GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glViewport(0, 0, window::GetWindowPointer()->GetWidth(), window::GetWindowPointer()->GetHeight());
}

void LightsManager::BuildLightsFrameBuffer()
{
	size_t lightNum = lights.size();
	FBOsLightsDepth.resize(lightNum);
	depthMapsLights.resize(lightNum);

	glGenFramebuffers(lightNum, FBOsLightsDepth.data());
	for (size_t i = 0; i < lightNum; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOsLightsDepth[i]);
		
		if (lights[i]->GetType() == LightType::LIGHT_Point)
		{
			depthMapsLights[i] = GenerateDepthCubeMap();
			glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapsLights[i], 0);
		}
		else
		{
			depthMapsLights[i] = GenerateDepthMap();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapsLights[i], 0);
		}
		glDrawBuffer(GL_NONE);						// Set Framebuffer Draw and Read to GL_NONE can explicitly tell OpenGL we are not going to draw anything
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

const glm::mat4& DirLight::BuildProjection()
{
	float near_plane = 1.0f, far_plane = 7.5f;
	return glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
}

const glm::mat4& DirLight::BuildView()
{
	return glm::lookAt(
		glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
}

const glm::mat4& SpotLight::BuildProjection()
{
	return glm::mat4();
}

const glm::mat4& SpotLight::BuildView()
{
	return glm::mat4();
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
