#include "GLRenderer.h"
#include "glad/glad.h"
#include "glfw3.h"

#include "Window.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

#include <cmath>

glRenderer::glRenderer(window* w)
{
	currentWindow = w;

	bHasInitilized = CreateShaderPrograms();

	triangle = mesh::GenerateTriangle();
	quad = mesh::GenerateQuad();

	tex = new texture("../Resources/Textures/brick.tga");
}

void glRenderer::Render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaders[0]->GetShaderProgram());
	glBindTexture(GL_TEXTURE_2D, tex->TextureID());
	triangle->Draw();
	//quad->Draw();

	glfwSwapBuffers(currentWindow->GetGLFWWindow());
}

bool glRenderer::CreateShaderPrograms()
{
	shader* s = new shader("../Shaders/vertex.glsl", "../Shaders/fragment.glsl");
	if (!s->HasInitialized()) return false;
	shaders.push_back(s);

	return true;
}
