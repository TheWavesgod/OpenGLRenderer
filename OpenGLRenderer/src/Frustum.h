#pragma once

#include "glm.hpp"
#include <gtc/matrix_transform.hpp>

class SceneNode;
class RenderObject;

class Plane
{
public:
	Plane(void) {}
	Plane(const glm::vec3& normal, float distance, bool normalise = false);
	~Plane(void) {}

	inline void SetNormal(const glm::vec3& val) { normal = val; }
	inline glm::vec3 GetNormal() const { return normal; }

	inline void SetDistance(float val) { distance = val; }
	inline float GetDistance() const { return distance; }

	bool ShpereInPlane(const glm::vec3& position, float radius) const;

protected:
	glm::vec3 normal;
	float distance;
};

class Frustum
{
public:
	Frustum(void) {}
	~Frustum() {}

	void FromMatrix(const glm::mat4& mat);
	bool InsideFrustum(SceneNode& n);
	bool InsideFrustum(RenderObject* object);

protected:
	Plane planes[6];
};

