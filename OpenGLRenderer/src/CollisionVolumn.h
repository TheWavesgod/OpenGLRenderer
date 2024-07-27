#pragma once

#include <glm.hpp>

enum class CollisionVolumeType
{
	AABB = 1,
	OBB = 2,
	Sphere = 4,
	Mesh = 8,
	Capsule = 16,
	Invalid = 256
};

class CollisionVolume
{
public:
	CollisionVolume() { type = CollisionVolumeType::Invalid; }

	~CollisionVolume() {}

	CollisionVolumeType type;
};

class AABBVolume : public CollisionVolume
{
public:
	AABBVolume(const glm::vec3& halfSizes) : halfSizes(halfSizes)
	{
		type = CollisionVolumeType::AABB;
	}

	~AABBVolume() {}

	glm::vec3 GetHalfSizes() const { return halfSizes; }

protected:
	glm::vec3 halfSizes;
};

class OBBVolume : public CollisionVolume
{
public:
	OBBVolume(const glm::vec3& halfSizes) : halfSizes(halfSizes)
	{
		type = CollisionVolumeType::OBB;
	}

	~OBBVolume(){}

	glm::vec3 GetHalfSizes() const { return halfSizes; }

protected:
	glm::vec3 halfSizes;
};

class SphereVolume : public CollisionVolume
{
public:
	SphereVolume(float sphereRadius = 1.0f) : radius(sphereRadius)
	{
		type = CollisionVolumeType::Sphere;
	}

	~SphereVolume(){}

	float GetRadius() const { return radius; }

protected:
	float radius;
};