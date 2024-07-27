#pragma once
#include <unordered_set>

#include "Level.h"
#include "CollisionDetection.h"

class PhysicsSystem
{
public:
	PhysicsSystem(Level& currentLevel);

	~PhysicsSystem();

	void Update(float dt);

protected:
	void CollisionDetection();
	void SpacialAcceleration();

	void IntegrateAcceleration(float dt);
	void InterrateVelocity(float dt);

	void UpdateGameObjectsBoundingAABB();

	Level& level;

	float dtOffset;

	glm::vec3 gravity;
	//std::unordered_set<CollisionDetection::CollisionInfo> allCollisions;


};

