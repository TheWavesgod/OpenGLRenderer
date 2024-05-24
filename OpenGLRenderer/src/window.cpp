#include "window.h"

window* window::windowPtr = nullptr;

float window::lastMouseX = 0.0f;
float window::lastMouseY = 0.0f;
float window::mouseInputX = 0.0f;
float window::mouseInputY = 0.0f;

window* window::CreateGLFWWindow(uint32_t width, uint32_t height, const std::string& title, bool bFullScreen)
{
	static window w(width, height, title, bFullScreen);
	windowPtr = &w;
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

	glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(w, mouse_callback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwWindow == nullptr;
	}

	glViewport(0, 0, width, height);
	this->width = width;
	this->height = height;
	lastMouseX = width * 0.5f;
	lastMouseY = height * 0.5f;
}

bool window::HasInitialized()
{
	return glfwWindow != nullptr;
}

bool window::Update(float& deltaT)
{
	if (glfwWindowShouldClose(glfwWindow))
	{
		return false;
	}

	// Calculate delta time
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	deltaT = deltaTime;

	UpdateInput();
	glfwPollEvents();

	return true;
}

void window::UpdateInput()
{
	mouseInputX = 0.0f;
	mouseInputY = 0.0f;
}

int window::GetInputState(int key)
{
	return glfwGetKey(windowPtr->GetGLFWWindow(), key);
}

void window::SetWindowShouldClose(bool bShouldClose)
{
	glfwSetWindowShouldClose(glfwWindow, bShouldClose);
}

void window::CloseWindow()
{
	glfwTerminate();
}

void window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouseInputX = lastMouseX - xpos;
	mouseInputY = lastMouseY - ypos; // reversed since y-coordinates range from bottom to top
	lastMouseX = xpos;
	lastMouseY = ypos;

	const float sensitivity = 0.1f;
	mouseInputX *= sensitivity;
	mouseInputY *= sensitivity;
}
