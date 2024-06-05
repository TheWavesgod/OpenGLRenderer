#include "window.h"
#include "glRenderer.h"
#include "shader.h"
#include "Level.h"

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

	Level level(renderer);

	float deltaT;
	while (w->Update(deltaT))
	{
		if (w->GetInputState(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			w->SetWindowShouldClose(true);
		}

		level.Update(deltaT);
	}

	w->CloseWindow();

	return 0;
}
