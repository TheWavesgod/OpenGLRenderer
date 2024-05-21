#include "Camera.h"
#include "window.h"
#include <algorithm>

void Camera::UpdateCamera(float dt)
{
	using namespace glm;

	yaw += window::mouseInputX;
	pitch += window::mouseInputY;

    pitch = std::min(pitch, 89.0f);
    pitch = std::max(pitch, -89.0f);
	if (yaw < 0)
	{
		yaw += 360.0f;

	}
	if (yaw > 360.0f)
	{
		yaw -= 360.0f;
	}
	transform.SetRotation(vec3(pitch, yaw, 0.0f));

	float speed = cameraMoveSpeed * dt;

	if (window::GetInputState(GLFW_KEY_W) == GLFW_PRESS) {}
	if (window::GetInputState(GLFW_KEY_S) == GLFW_PRESS) {}
	if (window::GetInputState(GLFW_KEY_A) == GLFW_PRESS) {}
	if (window::GetInputState(GLFW_KEY_D) == GLFW_PRESS) {}
}

glm::mat4 Camera::BuildViewMatrix()
{
	glm::mat4 view = glm::lookAt(transform.GetPosition(), transform.GetPosition() + transform.GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));

	return view;
}

glm::mat4 Camera::BuildProjectionMatrix()
{
	float width = window::GetWindowPointer()->GetWindowWidth();
	float height = window::GetWindowPointer()->GetWindowHeight();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 1000.0f);
	return projection;
}
