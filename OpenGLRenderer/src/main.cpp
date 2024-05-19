#include "window.h"

int main()
{
	window* w = window::CreateGLFWWindow(800, 600, "OpenGL Renderer", false);
	if (!w->HasInitialized())
	{
		return -1;
	}

	while (w->Update())
	{
		if (w->GetInputState(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			w->SetWindowShouldClose(true);
		}
	}

	w->CloseWindow();

	return 0;
}
