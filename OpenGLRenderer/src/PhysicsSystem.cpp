#include "PhysicsSystem.h"

#include "GameObject.h"
#include "PhysicsObject.h"
#include "Octree.h"

PhysicsSystem::PhysicsSystem(Level& currentLevel) : level(currentLevel)
{

}

PhysicsSystem::~PhysicsSystem()
{
}


const int idealHZ = 120;
const float idealDT = 1.0f / idealHZ;

int realHZ = idealHZ;
int realDT = idealDT;

void PhysicsSystem::Update(float dt)
{
	dtOffset += dt;

	UpdateGameObjectsBoundingAABB();

	int iteratorCount = 0;
	while (dtOffset > realDT)
	{
		IntegrateAcceleration(realDT);
		CollisionDetection();
		IntegrateVelocity(dt);

		dtOffset -= realDT;
		++iteratorCount;
	}
}

void PhysicsSystem::CollisionDetection()
{
	SpacialAcceleration();
	for (auto& i : spatialCollisions)
	{

	}
}

void PhysicsSystem::SpacialAcceleration()
{
	spatialCollisions.clear();
	Octree<GameObject*> tree(glm::vec3(1024, 1024, 1024), 7, 6);		// Set Octree

	GameObjectIterator begin;
	GameObjectIterator end;
	level.GetGameObjectsIterators(begin, end);
	for (GameObjectIterator i = begin; i != end; ++i)
	{
		glm::vec3 size;
		if (!(*i)->GetBoundingAABB(size)) continue;
		glm::vec3 pos = (*i)->GetTransform().GetPosition();
		tree.Insert(*i, pos, size);
	}

	tree.OperateOnContents(
		[this](std::list<OctreeEntry<GameObject*>>& contents)
		{
			CollisionDetection::CollisionInfo info;
			for (auto i = contents.begin(); i != contents.end(); ++i)
			{
				for(auto j = std::next(i); j != contents.end(); ++j)
				{
					info.a = std::min(i->object, j->object);
					info.b = std::max(i->object, j->object);
					spatialCollisions.insert(info);
				}
			}
		}
	);
}

void PhysicsSystem::IntegrateAcceleration(float dt)
{
	GameObjectIterator begin, end;
	level.GetGameObjectsIterators(begin, end);

	for (GameObjectIterator i = begin; i != end; ++i)
	{
		PhysicsObject* physicsObject = (*i)->GetPhysicsObject();
		if (physicsObject == nullptr) continue;					// No physics need to be calculate

		float inverseMass = physicsObject->GetInverseMass();
		if(inverseMass == 0) continue;							// static object

		glm::vec3 linearVel = physicsObject->GetLinearVelocity();
		glm::vec3 force = physicsObject->GetForce();
		glm::vec3 accel = force * inverseMass;

		if (physicsObject->HasGravity())
		{
			accel += gravity;
		}

		linearVel += accel * dt;
		physicsObject->SetLinearVelocity(linearVel);

		glm::vec3 angularVel = physicsObject->GetAngularVelocity();
		glm::vec3 torque = physicsObject->GetTorque();
		physicsObject->UpdateInertiaTensor();
		glm::vec3 angluarAccel = physicsObject->GetInverseInertiaTensor() * torque;
		angularVel += angluarAccel * dt;
	}
}

void PhysicsSystem::IntegrateVelocity(float dt)
{
	GameObjectIterator begin, end;
	level.GetGameObjectsIterators(begin, end);

	for (GameObjectIterator i = begin; i != end; ++i)
	{
		PhysicsObject* physicsObject = (*i)->GetPhysicsObject();
		if (physicsObject == nullptr) continue;

		Transform& transform = (*i)->GetTransform();

		// Calculate position
		glm::vec3 position = transform.GetPosition();
		glm::vec3 linearVel = physicsObject->GetLinearVelocity();
		position += linearVel * dt;
		transform.SetPosition(position);
		
		// Linear damping
		float frameLinearDamping = 1.0f - (physicsObject->GetLinearDamping() * dt);
		linearVel *= frameLinearDamping;
		physicsObject->SetLinearVelocity(linearVel);

		// Orientation
		glm::quat orientation = transform.GetQuatRotation();
		glm::vec3 angularVel = physicsObject->GetAngularVelocity();

		float angle = glm::length(angularVel) * dt;
		if (angle != 0.0f)
		{
			glm::vec3 axis = glm::normalize(angularVel);
			glm::quat deltaRot = glm::angleAxis(angle, axis);
			orientation = deltaRot * orientation;
			orientation = glm::normalize(orientation);
		}
		transform.SetRotation(orientation);

		// Angular damping
		float frameAngularDamping = 1.0f - (physicsObject->GetAngularDamping() * dt);
		angularVel *= frameAngularDamping;
		physicsObject->SetAngularVelocity(angularVel);
	}
}

void PhysicsSystem::UpdateGameObjectsBoundingAABB()
{
	GameObjectIterator begin;
	GameObjectIterator end;
	level.GetGameObjectsIterators(begin, end);
	for (GameObjectIterator i = begin; i != end; ++i)
	{
		(*i)->UpdateCollisionVolumeAABB();
	}
}
