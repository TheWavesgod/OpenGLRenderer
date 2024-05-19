#pragma once

#include "glfw3.h"
#include "glad/glad.h"
#include <iostream>
#include <string>

class window
{
public:
	static window* CreateGLFWWindow(uint32_t width = 800, uint32_t height = 600, const std::string& title = "GLFW", bool bFullScreen = false);

	bool HasInitialized();
	bool Update();

	int GetInputState(int key);

	void SetWindowShouldClose(bool bShouldClose);
	void CloseWindow();

	window(const window&) = delete;
	window& operator=(const window&) = delete;

protected:

private:
	GLFWwindow* glfwWindow;
	
	window(uint32_t width, uint32_t height, const std::string& title, bool bFullScreen);
};

