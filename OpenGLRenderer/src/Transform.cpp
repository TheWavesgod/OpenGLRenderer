#include "Transform.h"

void Transform::SetPosition(const glm::vec3& pos)
{
	position = pos;
	updateTransformMatrix();
}

void Transform::SetRotation(const glm::vec3& eulerAngles)
{
	rotation = glm::quat(glm::radians(eulerAngles));
	updateTransformMatrix();
}

void Transform::SetScale(const glm::vec3& scl)
{
	scale = scl;
	updateTransformMatrix();
}

void Transform::updateTransformMatrix()
{
	transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, position);
	transformMatrix *= glm::toMat4(rotation);
	transformMatrix = glm::scale(transformMatrix, scale);
}
