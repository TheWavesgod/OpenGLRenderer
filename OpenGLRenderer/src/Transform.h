#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/quaternion.hpp>
#include <iostream>

class Transform
{
public:
	Transform() :
		position(0.0f, 0.0f, 0.0f), 
		rotation(glm::quat_identity<float, glm::packed_highp>()),
		scale(1.0f, 1.0f, 1.0f) 
	{
		updateTransformMatrix();
	}

	void SetPosition(const glm::vec3& pos);
	void SetRotation(const glm::vec3& eulerAngles);
	void SetScale(const glm::vec3& scl);
	
	inline const glm::vec3& GetPosition() const { return position; }
	inline const glm::vec3& GetRotation() const { return glm::degrees(glm::eulerAngles(rotation)); }
	inline const glm::vec3& GetScale() const { return scale; }
	inline const glm::mat4& GetTransMatrix() const { return transformMatrix; }

	inline const glm::vec3& GetForwardVector() const { return glm::normalize(rotation * glm::vec3(0.0f, 0.0f, -1.0f)); }
	inline const glm::vec3& GetRightVector() const { return glm::normalize(rotation * glm::vec3(1.0f, 0.0f, 0.0f)); }
	inline const glm::vec3& GetUpVector() const { return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f)); }

	const Transform& operator*(const Transform& other) const;

private:
	void updateTransformMatrix();

private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	glm::mat4 transformMatrix;
};

