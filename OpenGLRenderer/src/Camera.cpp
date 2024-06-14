#include "Camera.h"
#include "window.h"
#include <algorithm>

void Camera::UpdateCamera(float dt)
{
	using namespace glm;

	const float sensitive = 0.1f;

	yaw -= window::mouseInputX * sensitive;
	pitch += window::mouseInputY * sensitive;

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

	cameraMoveSpeed = std::clamp(cameraMoveSpeed + window::mouseScroll, 10.0f, 50.0f);

	float displacement = cameraMoveSpeed * dt;

	glm::vec3 moveDir = glm::vec3(0.0f);
	if (window::GetInputState(GLFW_KEY_W) == GLFW_PRESS) { moveDir += transform.GetForwardVector(); }
	if (window::GetInputState(GLFW_KEY_S) == GLFW_PRESS) { moveDir += -transform.GetForwardVector(); }
	if (window::GetInputState(GLFW_KEY_A) == GLFW_PRESS) { moveDir += -transform.GetRightVector(); }
	if (window::GetInputState(GLFW_KEY_D) == GLFW_PRESS) { moveDir += transform.GetRightVector(); }
	if (glm::length(moveDir) != 0.0f)
	{
		glm::vec3 moveDistance = glm::normalize(moveDir) * displacement;
		transform.SetPosition(transform.GetPosition() + moveDistance);
	}
	moveDir = glm::vec3(0.0f);
	if (window::GetInputState(GLFW_KEY_Q) == GLFW_PRESS) { moveDir += -transform.GetUpVector(); }
	if (window::GetInputState(GLFW_KEY_E) == GLFW_PRESS) { moveDir += transform.GetUpVector(); }
	if (glm::length(moveDir) != 0.0f)
	{
		glm::vec3 moveDistance = glm::normalize(moveDir) * displacement;
		transform.SetPosition(transform.GetPosition() + moveDistance);
	}
}

glm::mat4 Camera::BuildViewMatrix()
{
	glm::mat4 view = glm::lookAt(transform.GetPosition(), transform.GetPosition() + transform.GetForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));

	return view;
}

glm::mat4 Camera::BuildProjectionMatrix()
{
	float width = window::GetWindowPointer()->GetWidth();
	float height = window::GetWindowPointer()->GetHeight();

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 1000.0f);
	return projection;
}

void Camera::UploadProjectionMatrix(GLuint uniformBuffer)
{
	// Send projection Mat4 to uniform buffer once
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(this->BuildProjectionMatrix()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::UploadViewMatrix(GLuint uniformBuffer)
{
	// Send view mat4 to uniform buffer every frame before drawing
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(this->BuildViewMatrix()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::UploadViewPos(GLuint uniformBuffer)
{
	// Send view position to uniform buffer every frame before drawing
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), glm::value_ptr(this->transform.GetPosition()));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
