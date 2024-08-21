#include "UserInterface.h"
#include "window.h"
#include "glRenderer.h"
#include "Level.h"

#include <chrono>

int main()
{
	// Record launch time
	auto start_time = std::chrono::high_resolution_clock::now();

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

	// launch time
	auto start_loop_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> startup_duration = start_loop_time - start_time;
	renderer->launch_time = startup_duration.count();

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
