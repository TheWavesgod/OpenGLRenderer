#include "GLRenderer.h"

#include "Window.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "CubeMap.h"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include <cmath>


glRenderer::glRenderer(window* w)
{
	currentWindow = w;

	bHasInitilized = CreateShaderPrograms();

	cube = Mesh::GenerateCube();

	tex = new Texture("../Resources/Textures/brick.tga");

	skybox = new CubeMap(
		"../Resources/CubeMap/evening_right.jpg",
		"../Resources/CubeMap/evening_left.jpg",
		"../Resources/CubeMap/evening_top.jpg",
		"../Resources/CubeMap/evening_bottom.jpg",
		"../Resources/CubeMap/evening_front.jpg",
		"../Resources/CubeMap/evening_back.jpg"
	);

	InitializeRenderer();
}

glRenderer::~glRenderer(void)
{
	delete cube;
	for (auto shader : shaders)
	{
		delete shader;
	}
}

void glRenderer::InitializeRenderer()
{
	GenerateScreenQuad();
	CreateFrameBuffer();
	CreateUniformBuffer();

	glEnable(GL_MULTISAMPLE);

	// Send projection Mat4 to uniform buffer once
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(camera->BuildProjectionMatrix()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void glRenderer::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBOMultiSample);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	DrawScene();

	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBOMultiSample);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBOPostProcess);
	glBlitFramebuffer(0, 0, currentWindow->GetWidth(), currentWindow->GetHeight(), 0, 0, currentWindow->GetWidth(), currentWindow->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// Gaussian Blur
	bool horizontal = true, first_iteration = true;
	int amount = 10;
	glUseProgram(shaders[2]->GetShaderProgram());
	for (unsigned int i = 0; i < amount; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOpingpong[horizontal]);
		//shaders[2]->SetUniformInt("horizontal", horizontal);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? colorTexPostProcess[1] : colorTexPingpong[!horizontal]); // First iteration bind the bright Texture From last render
		// Draw
		horizontal = !horizontal;
		if (first_iteration) first_iteration = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaders[2]->GetShaderProgram());
	shaders[2]->SetUniformInt("screenTexture", 0);
	shaders[2]->SetUniformInt("gammaCorrection", false);
	shaders[2]->SetUniformFloat("exposure", 1.0f);
	glBindVertexArray(screenQuadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, colorTexPostProcess[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glfwSwapBuffers(currentWindow->GetGLFWWindow());
}

void glRenderer::DrawScene()
{
	// Send view mat4 to uniform buffer every frame before drawing
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->BuildViewMatrix()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glUseProgram(shaders[0]->GetShaderProgram());
	glBindTexture(GL_TEXTURE_2D, tex->TextureID());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	shaders[0]->SetUniformMat4("model", model);

	cube->Draw(*shaders[0]);

	// Draw Skybox
	glUseProgram(shaders[3]->GetShaderProgram());
	skybox->Draw(*shaders[3]);
}

bool glRenderer::CreateShaderPrograms()
{
	Shader* s = new Shader("../Shaders/vertex.glsl", "../Shaders/fragment.glsl");
	if (!s->HasInitialized()) return false;
	shaders.push_back(s);

	s = new Shader("../Shaders/vertPhoneLight.glsl", "../Shaders/fragPhonLight.glsl");
	if (!s->HasInitialized()) return false;
	shaders.push_back(s);

	s = new Shader("../Shaders/vertPostProcessing.glsl", "../Shaders/fragPostProcessing.glsl");
	if (!s->HasInitialized()) return false;
	shaders.push_back(s);

	s = new Shader("../Shaders/vertCubemap.glsl", "../Shaders/fragCubemap.glsl");
	if (!s->HasInitialized()) return false;
	shaders.push_back(s);

	return true;
}

void glRenderer::CreateFrameBuffer()
{
	glGenFramebuffers(1, &FBOPostProcess);
	glBindFramebuffer(GL_FRAMEBUFFER, FBOPostProcess);

	// Generate texture
	glGenTextures(2, colorTexPostProcess);
	for(int i = 0; i < 2; ++i)
	{
		// Create texture for color buffer and bright color buffer of bloom
		glBindTexture(GL_TEXTURE_2D, colorTexPostProcess[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, currentWindow->GetWidth(), currentWindow->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);
		// Attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorTexPostProcess[i], 0);
	}

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, currentWindow->GetWidth(), currentWindow->GetHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER::POSTPROCESSING Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Multisample framebuffer		// TODO: Check Multisample has any effect on HDR
	glGenFramebuffers(1, &FBOMultiSample); 
	glBindFramebuffer(GL_FRAMEBUFFER, FBOMultiSample);

	glGenTextures(1, &colorTexMultiSample);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorTexMultiSample);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB16F, currentWindow->GetWidth(), currentWindow->GetHeight(), GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorTexMultiSample, 0);

	GLuint rboMultisample;
	glGenRenderbuffers(1, &rboMultisample);
	glBindRenderbuffer(GL_RENDERBUFFER, rboMultisample);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, currentWindow->GetWidth(), currentWindow->GetHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboMultisample);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout<<"ERROR::FRAMEBUFFER::MULTISAMPLE Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// For blurring an image, create ping-pong buffer
	glGenFramebuffers(2, FBOpingpong);
	glGenTextures(2, colorTexPingpong);
	for (unsigned int i = 0; i < 2; ++i)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBOpingpong[i]);
		glBindTexture(GL_TEXTURE_2D, colorTexPingpong[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, currentWindow->GetWidth(), currentWindow->GetHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexPingpong[i], 0);
	}
}

void glRenderer::CreateUniformBuffer()
{
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
}

void glRenderer::GenerateScreenQuad()
{
	float screenQuadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
	};

	GLuint screenQuadVBO;
	glGenBuffers(1, &screenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), &screenQuadVertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &screenQuadVAO);
	glBindVertexArray(screenQuadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}