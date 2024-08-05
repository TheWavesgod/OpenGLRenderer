#include "window.h"

window* window::windowPtr = nullptr;

bool window::firstMouse = true;
float window::lastMouseX = 0.0f;
float window::lastMouseY = 0.0f;
float window::mouseInputX = 0.0f;
float window::mouseInputY = 0.0f;
float window::mouseScroll = 0.0f;
bool window::bEnableMouseCursor = true;

window* window::CreateGLFWWindow(uint32_t width, uint32_t height, const std::string& title, bool bFullScreen)
{
	static window w(width, height, title, bFullScreen);
	windowPtr = &w;
    return &w;
}

window::window(uint32_t width, uint32_t height, const std::string& title, bool bFullScreen)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);


	GLFWwindow* w = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (w == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		glfwWindow == nullptr;
	}
	glfwMakeContextCurrent(w);
	glfwWindow = w;

	glfwSetCursorPosCallback(w, mouse_callback);
	glfwSetMouseButtonCallback(w, mousebuttom_callback);
	glfwSetScrollCallback(w, scroll_callback);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwWindow = nullptr;
	}

	int flags; 
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		std::cout << "Enable Debug Output" << std::endl;
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
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

void window::SwitchMouseInput()
{
	if (bEnableMouseCursor)
	{
		firstMouse = true;
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	bEnableMouseCursor = !bEnableMouseCursor;
}

void window::SetMouseCursorState(bool bEnable)
{
	if (bEnable)
	{
		bEnableMouseCursor = true;
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		firstMouse = true;
		glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		bEnableMouseCursor = false;
	}
}

void window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (window::bEnableMouseCursor) return;

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	mouseInputX = xpos - lastMouseX;
	mouseInputY = lastMouseY - ypos; // reversed since y-coordinates range from bottom to top

	lastMouseX = xpos;
	lastMouseY = ypos;
}

void window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mouseScroll = yoffset;
}

void window::mousebuttom_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			window::GetWindowPointer()->SetMouseCursorState(false);
		}
		else if (action == GLFW_RELEASE)
		{
			window::GetWindowPointer()->SetMouseCursorState(true);
		}
	}
}

void window::glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}
