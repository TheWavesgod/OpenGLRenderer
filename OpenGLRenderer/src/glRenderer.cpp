#include "GLRenderer.h"
#include "glad/glad.h"
#include "glfw3.h"

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

	cube = Mesh::GenerateQuad();

	camera = new Camera();

	tex = new Texture("../Resources/Textures/brick.tga");
}

glRenderer::~glRenderer(void)
{
	delete cube;
	delete camera;

	for (auto shader : shaders)
	{
		delete shader;
	}
}

void glRenderer::Render()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->UpdateCamera();

	glUseProgram(shaders[0]->GetShaderProgram());
	glBindTexture(GL_TEXTURE_2D, tex->TextureID());
	
	shaders[0]->SetUniformMat4("projection", camera->BuildProjectionMatrix());
	shaders[0]->SetUniformMat4("view", camera->BuildViewMatrix());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
	shaders[0]->SetUniformMat4("model", model);

	cube->Draw(*shaders[0]);

	glfwSwapBuffers(currentWindow->GetGLFWWindow());
}

bool glRenderer::CreateShaderPrograms()
{
	Shader* s = new Shader("../Shaders/vertex.glsl", "../Shaders/fragment.glsl");
	if (!s->HasInitialized()) return false;
	shaders.push_back(s);

	s = new Shader("../Shaders/vertPhoneLight.glsl", "../Shaders/fragPhonLight.glsl");
	if (!s->HasInitialized()) return false;
	shaders.push_back(s);

	return true;
}
