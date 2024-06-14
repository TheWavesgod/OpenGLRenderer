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
	static float mouseScroll;

	bool HasInitialized();
	bool Update(float& deltaT);

	void SetWindowShouldClose(bool bShouldClose);
	void CloseWindow();

	window(const window&) = delete;
	window& operator=(const window&) = delete;

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

protected:
	float deltaTime;
	float lastFrame;

	GLuint width;
	GLuint height;
private:
	static window* windowPtr;
	
	static bool firstMouse;
	static float lastMouseX;
	static float lastMouseY;

	GLFWwindow* glfwWindow;
	
	window(uint32_t width, uint32_t height, const std::string& title, bool bFullScreen);

	void UpdateInput();

public:
	inline GLFWwindow* GetGLFWWindow() const { return glfwWindow; }
	inline GLuint GetWidth() const { return width; }
	inline GLuint GetHeight() const { return height; }
};


