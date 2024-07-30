#pragma once

#include "Transform.h"

class PhysicsObject;
class CollisionVolume;

class GameObject
{
public:
	GameObject(CollisionVolume* newVolumn, PhysicsObject* newPhysicsObject);
	~GameObject();

	void UpdateCollisionVolumeAABB();
	void UpdateCollisionVolumeSphere();

	Transform& GetTransform() { return transform; }
	PhysicsObject* GetPhysicsObject() const { return physicsObject; }
	const CollisionVolume* GetBoundingVolume() const { return volume; }

	float GetBoundingSphere() const { return BoundingSphere; }
	bool GetBoundingAABB(glm::vec3& size) const;

protected:
	glm::vec3 GetBoundingAABBForOBB();

protected:
	Transform transform;

	unsigned int worldID;
	std::string name;

	CollisionVolume* volume = nullptr;
	PhysicsObject* physicsObject = nullptr;

	glm::vec3 BoundingAABB;
	float BoundingSphere = 10.0f;
};

