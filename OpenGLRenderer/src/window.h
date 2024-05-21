#pragma once

#include "glfw3.h"
#include "glad/glad.h"
#include <iostream>
#include <string>

class window
{
public:
	static window* CreateGLFWWindow(uint32_t width = 800, uint32_t height = 600, const std::string& title = "GLFW", bool bFullScreen = false);
	static window* GetWindowPointer() { return windowPtr; }

	static int GetInputState(int key);

	static float mouseInputX;
	static float mouseInputY;

	bool HasInitialized();
	bool Update(float& deltaT);

	void SetWindowShouldClose(bool bShouldClose);
	void CloseWindow();

	window(const window&) = delete;
	window& operator=(const window&) = delete;

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

protected:
	float deltaTime;
	float lastFrame;

	GLuint width;
	GLuint height;
private:
	static window* windowPtr;
	
	static float lastMouseX;
	static float lastMouseY;

	GLFWwindow* glfwWindow;
	
	window(uint32_t width, uint32_t height, const std::string& title, bool bFullScreen);

public:
	inline GLFWwindow* GetGLFWWindow() const { return glfwWindow; }
	inline GLuint GetWindowWidth() const { return width; }
	inline GLuint GetWindowHeight() const { return height; }
};


