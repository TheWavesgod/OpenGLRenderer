#pragma once
#include <glm.hpp>

struct RayCollision
{
	void* node;
	glm::vec3 collidedAt;
	float rayDistance;

	RayCollision(void* node, glm::vec3 collidedAt) : collidedAt(collidedAt)
	{
		this->node = node;
		rayDistance = 0.0f;
	}

	RayCollision()
	{
		node = nullptr;
		collidedAt = glm::vec3(0.0f);
		rayDistance = FLT_MAX;
	}
};

class Ray
{
public:
	Ray(const glm::vec3& position, glm::vec3& direction) : pos(position), dir(direction) {}
	~Ray() {}

	glm::vec3 GetPosition() const { return pos; }
	glm::vec3 GetDirection() const { return dir; }

protected:
	glm::vec3 pos;
	glm::vec3 dir;
};