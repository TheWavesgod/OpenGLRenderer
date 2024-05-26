#include "window.h"
#include "glRenderer.h"
#include "shader.h"
#include "Camera.h"

int main()
{
	window* w = window::CreateGLFWWindow(800, 600, "OpenGL Renderer", false);
	if (!w->HasInitialized())
	{
		return -1;
	}

	glRenderer* renderer = new glRenderer(w);
	if (!renderer->HasInitialized())
	{
		return -1;
	}

	Camera camera(0.0f, 0.0f, glm::vec3(0.0f, 0.0f, 3.0f));
	renderer->camera = &camera;

	float deltaT;
	while (w->Update(deltaT))
	{
		if (w->GetInputState(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			w->SetWindowShouldClose(true);
		}

		camera.UpdateCamera(deltaT);
		renderer->Render();
	}

	w->CloseWindow();

	return 0;
}
