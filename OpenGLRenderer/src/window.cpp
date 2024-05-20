#include "window.h"

window* window::CreateGLFWWindow(uint32_t width, uint32_t height, const std::string& title, bool bFullScreen)
{
	static window w(width, height, title, bFullScreen);
    return &w;
}

window::window(uint32_t width, uint32_t height, const std::string& title, bool bFullScreen)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* w = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (w == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		glfwWindow == nullptr;
	}
	glfwMakeContextCurrent(w);
	glfwWindow = w;

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwWindow == nullptr;
	}

	glViewport(0, 0, width, height);
}

bool window::HasInitialized()
{
	return glfwWindow != nullptr;
}

bool window::Update()
{
	if (glfwWindowShouldClose(glfwWindow))
	{
		return false;
	}

	glfwPollEvents();

	return true;
}

int window::GetInputState(int key)
{
	return glfwGetKey(glfwWindow, key);
}

void window::SetWindowShouldClose(bool bShouldClose)
{
	glfwSetWindowShouldClose(glfwWindow, bShouldClose);
}

void window::CloseWindow()
{
	glfwTerminate();
}
