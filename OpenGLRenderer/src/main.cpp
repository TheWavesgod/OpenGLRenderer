#include "UserInterface.h"
#include "window.h"
#include "glRenderer.h"
#include "Level.h"

int main()
{
	window* w = window::CreateGLFWWindow(2560, 1440, "OpenGL Renderer", false);
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

	UserInterface* ui = UserInterface::CreateUserInterface(w, renderer, level);
	if (!ui)
	{
		return -1;
	}

	float deltaT;
	while (w->Update(deltaT))
	{
		if (w->GetInputState(GLFW_KEY_TAB) == GLFW_PRESS)
		{
			w->SwitchMouseInput();
		}

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ui->StartNewFrame();

		level.Update(deltaT);

		ui->Render();

		glfwSwapBuffers(w->GetGLFWWindow());
	}
	
	ui->Shutdown();
	w->CloseWindow();
	delete renderer;

	return 0;
}
