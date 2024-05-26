#include "GLRenderer.h"

#include "Window.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"

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
}

void glRenderer::GenerateScreenQuad()
{
	float screenQuadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
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

void glRenderer::Render()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	DrawScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaders[2]->GetShaderProgram());
	shaders[2]->SetUniformInt("screenTexture", 0);
	glBindVertexArray(screenQuadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(currentWindow->GetGLFWWindow());
}

void glRenderer::DrawScene()
{
	glUseProgram(shaders[0]->GetShaderProgram());
	glBindTexture(GL_TEXTURE_2D, tex->TextureID());

	shaders[0]->SetUniformMat4("projection", camera->BuildProjectionMatrix());
	shaders[0]->SetUniformMat4("view", camera->BuildViewMatrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	shaders[0]->SetUniformMat4("model", model);

	cube->Draw(*shaders[0]);
}

bool glRenderer::CreateShaderPrograms()
{
	Shader* s = new Shader("../Shaders/vertex.glsl", "../Shaders/fragment.glsl");
	if (!s->HasInitialized()) return false;
	shaders.push_back(s);

	s = new Shader("../Shaders/vertPhoneLight.glsl", "../Shaders/fragPhonLight.glsl");
	if (!s->HasInitialized()) return false;
	shaders.push_back(s);

	s = new Shader("../Shaders/vertFB.glsl", "../Shaders/fragFB.glsl");
	if (!s->HasInitialized()) return false;
	shaders.push_back(s);

	return true;
}

void glRenderer::CreateFrameBuffer()
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// Generate texture
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, currentWindow->GetWidth(), currentWindow->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, currentWindow->GetWidth(), currentWindow->GetHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
