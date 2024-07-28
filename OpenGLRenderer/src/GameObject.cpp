#include "GameObject.h"
#include "CollisionVolumn.h"

GameObject::GameObject(CollisionVolume* newVolumn, PhysicsObject* newPhysicsObject)
{
	volume = newVolumn;
	physicsObject = newPhysicsObject;
}

GameObject::~GameObject()
{
	delete volume;
	delete physicsObject;
}

void GameObject::UpdateCollisionVolumeAABB()
{
	if (volume == nullptr) return;

	switch (volume->type)
	{
	case CollisionVolumeType::AABB:
		BoundingAABB = static_cast<AABBVolume*>(volume)->GetHalfSizes();	//TODO: why can access private base class's type
		break;

	case CollisionVolumeType::OBB:
		glm::quat rot = transform.GetRotation();
		glm::vec3 halfsizes = static_cast<OBBVolume*>(volume)->GetHalfSizes();
		glm::vec3 newSizes = rot * halfsizes;
		BoundingAABB = glm::abs(newSizes);
		break;

	case CollisionVolumeType::Sphere:
		float radius = static_cast<SphereVolume*>(volume)->GetRadius();
		BoundingAABB = glm::vec3(radius, radius, radius);
		break;
	}
}

bool GameObject::GetBoundingAABB(glm::vec3& size) const
{
	if (volume == nullptr) return false;

	size = BoundingAABB;
	return true;
}
