#include "PhysicsObject.h"
#include "Transform.h"
#include "CollisionVolumn.h"

PhysicsObject::PhysicsObject(Transform* parentTransform, const CollisionVolume* parentVolume)
	: volume(parentVolume), transform(parentTransform)
{
	inverseMass = 1.0f;
}

PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::UpdateInertiaTensor()
{
	glm::quat rot = transform->GetQuatRotation();		// TODO: Update inertia tensor
	glm::quat inverseRot = glm::inverse(rot);

	inverseInertiaTensor = glm::toMat3(rot) * 
		glm::mat3(glm::scale(glm::mat4(1.0f), inverseInertia)) * glm::toMat3(inverseRot);
}

void PhysicsObject::InitCubeInertia()
{
	glm::vec3 dimensions = transform->GetScale();

	glm::vec3 fullWidth = dimensions * 2.0f;

	glm::vec3 dimensionsSq = fullWidth * fullWidth;

	inverseInertia.x = (12.0f * inverseMass) / (dimensionsSq.y + dimensionsSq.z);
	inverseInertia.y = (12.0f * inverseMass) / (dimensionsSq.x + dimensionsSq.z);
	inverseInertia.z = (12.0f * inverseMass) / (dimensionsSq.x + dimensionsSq.y);
}

void PhysicsObject::InitSphereInertia()
{
	glm::vec3 scale = transform->GetScale();
	float radius = std::max(std::max(scale.x, scale.y), scale.z);

	float i = 2.5f * inverseMass / (radius * radius);

	inverseInertia = glm::vec3(i, i, i);
}
