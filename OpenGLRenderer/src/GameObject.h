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

	PhysicsObject* GetPhysicsObject() const { return physicsObject; }
	Transform& GetTransform() { return transform; }

protected:
	Transform transform;

	unsigned int worldID;
	std::string name;

	CollisionVolume* volume = nullptr;
	PhysicsObject* physicsObject = nullptr;

	glm::vec3 BoundingAABB;
	float BoundingSphere;
};

