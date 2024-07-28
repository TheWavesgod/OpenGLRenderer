#pragma once
#include <glm.hpp>
#include "GameObject.h"

class CollisionDetection
{
public:

	struct ContactPoint
	{

	};

	struct CollisionInfo
	{
		GameObject* a;
		GameObject* b;

		int frameleft;
	};

	/**
	 * Collision detection
	 */
	static bool AABBTest(const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& halfSizeA, const glm::vec3& halfSizeB);

};