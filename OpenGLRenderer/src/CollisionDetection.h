#pragma once
#include <glm.hpp>
#include "GameObject.h"
#include "CollisionVolumn.h"
#include "Ray.h"
#include "Frustum.h"

class CollisionDetection
{
public:

	struct ContactPoint
	{
		glm::vec3 localA; // local in world space, not include rot
		glm::vec3 localB;
		glm::vec3 normal;
		float penetration;
	};

	struct CollisionInfo
	{
		GameObject* a;
		GameObject* b;

		int frameleft;

		ContactPoint point;

		void AddContactPoint(const glm::vec3 localA, const glm::vec3& localB, const glm::vec3& normal, float p)
		{
			point.localA = localA;
			point.localB = localB;
			point.normal = normal;
			point.penetration = p;
		}
		
		bool operator ==(const CollisionInfo& other) const { return a == other.a && b == other.b; }
	};

	static bool RayIntersection(const Ray& ray, GameObject& object, RayCollision& collision);
	static bool ObjectIntersection(GameObject* a, GameObject* b, CollisionInfo& collisionInfo);

	/**
	 * Collision detection
	 */
	static bool AABBTest(const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& halfSizeA, const glm::vec3& halfSizeB);
	
	static bool OBBTest(const glm::vec3& Axis, const glm::vec3& halfSizeA, const glm::vec3& rotA,
		const glm::vec3& halfSizeB, glm::vec3& rotB, float penetration, glm::vec3& collisionNormal);

	static bool AABBIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
		const AABBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo);

	static bool OBBIntersection(const OBBVolume& volumeA, const Transform& worldTransformA,
		const OBBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo);

	static bool SphereIntersection(const SphereVolume& volumeA, const Transform& worldTransformA,
		const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo);

	static bool AABBShpereIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
		const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo);

	static bool OBBShpereIntersection(const OBBVolume& volumeA, const Transform& worldTransformA,
		const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo);

	static bool AABBOBBIntersection(const AABBVolume& volumeA, const Transform& worldTransformA,
		const OBBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo);

	/**
	 * Ray Inspection
	 */
	static bool RayPlaneIntersection(const Ray& r, const Plane& plane, RayCollision& collision);

	static bool RayTriangleIntersection(const Ray& r, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, RayCollision& collision);

	static bool RayBoxIntersection(const Ray& r, const glm::vec3& boxPos, const glm::vec3& boxSize, RayCollision& collision);

	static bool RayAABBIntersection(const Ray& r, const AABBVolume& volume, const Transform& worldTransform, RayCollision& collision);

	static bool RayOBBIntersection(const Ray& r, const OBBVolume& volume, const Transform& worldTransform, RayCollision& collision);
	
	static bool RaySphereIntersection(const Ray& r, const SphereVolume& volume, const Transform& worldTransform, RayCollision& collision);

	/**
	 * Point 
	 */
	static bool IsPointInsideTriangle(const glm::vec3& point, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
};

namespace std
{
	template<>
	struct hash<CollisionDetection::CollisionInfo>
	{
		std::size_t operator()(const CollisionDetection::CollisionInfo& info) const
		{
			std::size_t h1 = std::hash<GameObject*>()(info.a);
			std::size_t h2 = std::hash<GameObject*>()(info.b);
			return h1 ^ (h2 << 1);
		}
	};
}