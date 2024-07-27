#pragma once
#include <glm.hpp>

class CollisionVolume;
class Transform;

class PhysicsObject
{
public:
	PhysicsObject(Transform* parentTransform, const CollisionVolume* parentVolume);
	~PhysicsObject();

	void UpdateInertiaTensor();

	void InitCubeInertia();
	void InitSphereInertia();

	/** Setter */
	void SetLinearVelocity(const glm::vec3& newVel) { linearVelocity = newVel; }
	void SetAngularVelocity(const glm::vec3& newVel) { angularVelocity = newVel; }

	/** Getter */
	inline float GetInverseMass() const { return inverseMass; }
	inline bool HasGravity() const { return bHasGravity; }
	inline glm::vec3 GetLinearVelocity() const { return linearVelocity; }
	inline glm::vec3 GetForce() const { return force; }
	inline glm::vec3 GetAngularVelocity() const { return angularVelocity; }
	inline glm::vec3 GetTorque() const { return torque; }
	inline glm::mat3 GetInverseInertiaTensor() const { return inverseInertiaTensor; }
	inline float GetLinearDamping() const { return linearDamping; }
	inline float GetAngularDamping() const { return anguleDamping; }

protected:
	const CollisionVolume* volume;
	Transform* transform;

	float inverseMass;
	glm::vec3 inverseInertia;
	glm::mat3 inverseInertiaTensor;
	
	float elasticity = 0.8f;
	float friction = 0.8f;
	bool bHasGravity = false;

	float linearDamping = 0.1f;
	float anguleDamping = 0.1f;

	glm::vec3 linearVelocity;
	glm::vec3 force;

	glm::vec3 angularVelocity;
	glm::vec3 torque;
};

