#include "Frustum.h"
#include "SceneNode.h"
#include "RenderObject.h"

Plane::Plane(const glm::vec3& normal, float distance, bool normalise)
{
	if (normalise)
	{
		float length = glm::length(normal);
		if (length > 0)
		{
			this->normal = normal / length;
			this->distance = distance / length;
		}
	}
	else
	{
		this->normal = normal;
		this->distance = distance;
	}
}

bool Plane::ShpereInPlane(const glm::vec3& position, float radius) const
{
	if (glm::dot(position, normal) - distance + radius <= 0)
	{
		return false;
	}
	return true;
}

void Frustum::FromMatrix(const glm::mat4& mat)
{
	glm::vec3 xAxis = glm::vec3(mat[0][0], mat[1][0], mat[2][0]);
	glm::vec3 yAxis = glm::vec3(mat[0][1], mat[1][1], mat[2][1]);
	glm::vec3 zAxis = glm::vec3(mat[0][2], mat[1][2], mat[2][2]);
	glm::vec3 wAxis = glm::vec3(mat[0][3], mat[1][3], mat[2][3]);

	// Right
	planes[0] = Plane(wAxis - xAxis, mat[3][3] - mat[3][0], true);

	// Left
	planes[1] = Plane(wAxis + xAxis, mat[3][3] + mat[3][0], true);

	// Bottom
	planes[2] = Plane(wAxis + yAxis, mat[3][3] + mat[3][1], true);

	// Top
	planes[3] = Plane(wAxis - yAxis, mat[3][3] - mat[3][1], true);

	// Near
	planes[4] = Plane(wAxis + zAxis, mat[3][3] - mat[3][2], true);

	// Far
	planes[5] = Plane(wAxis - zAxis, mat[3][3] - mat[3][2], true);
}

bool Frustum::InsideFrustum(SceneNode& n)
{
	for (unsigned int i = 0; i < 6; ++i)
	{
		if (!planes[i].ShpereInPlane(n.GetWorldTransform().GetPosition(), n.GetBoundingRadius()))
		{
			return false;
		}
	}
	return true;
}

bool Frustum::InsideFrustum(RenderObject* o)
{
	for (unsigned int i = 0; i < 6; ++i)
	{
		if (!planes[i].ShpereInPlane(o->GetTrasform().GetPosition(), o->GetBoundingSphere()))
		{
			return false;
		}
	}
	return true;
}
