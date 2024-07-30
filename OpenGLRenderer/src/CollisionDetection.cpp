#include "CollisionDetection.h"

bool CollisionDetection::RayIntersection(const Ray& ray, GameObject& object, RayCollision& collision)
{
    const Transform& worldTransform = object.GetTransform();
    const CollisionVolume* volume = object.GetBoundingVolume();
    
    if (volume == nullptr) return false;

    switch (volume->type)
    {
    case CollisionVolumeType::AABB:
        break;
    }
}

bool CollisionDetection::ObjectIntersection(GameObject* a, GameObject* b, CollisionInfo& collisionInfo)
{
    const CollisionVolume* volA = a->GetBoundingVolume();
    const CollisionVolume* volB = b->GetBoundingVolume();

    if (!volA || !volB) return false;

    collisionInfo.a = a;
    collisionInfo.b = b;

    Transform& transformA = a->GetTransform();
    Transform& transformB = b->GetTransform();

    CollisionVolumeType pairType = (CollisionVolumeType)((int)volA->type | (int)volB->type);
    if (pairType == CollisionVolumeType::AABB)
    {
        return AABBIntersection((AABBVolume&)(*volA), transformA, (AABBVolume&)(*volB), transformB, collisionInfo);
    }
    if (pairType == CollisionVolumeType::OBB)
    {
        return OBBIntersection((OBBVolume&)(*volA), transformA, (OBBVolume&)(*volB), transformB, collisionInfo);
    }
    if (pairType == CollisionVolumeType::Sphere)
    {
        return SphereIntersection((SphereVolume&)(*volA), transformA, (SphereVolume&)(*volB), transformB, collisionInfo);
    }

    // AABB vs Sphere pairs
    if (volA->type == CollisionVolumeType::AABB && volB->type == CollisionVolumeType::Sphere)
    {
        return AABBShpereIntersection((AABBVolume&)(*volA), transformA, (SphereVolume&)(*volB), transformB, collisionInfo);
    }
    if (volA->type == CollisionVolumeType::Sphere && volB->type == CollisionVolumeType::AABB)
    {
        collisionInfo.a = b;
        collisionInfo.b = a;
        return AABBShpereIntersection(*((AABBVolume*)(volB)), transformB, (SphereVolume&)(*volA), transformA, collisionInfo);
    }

    // OBB vs Sphere pairs
    if (volA->type == CollisionVolumeType::OBB && volB->type == CollisionVolumeType::Sphere)
    {
        return OBBShpereIntersection((OBBVolume&)(*volA), transformA, (SphereVolume&)(*volB), transformB, collisionInfo);
    }
	if (volA->type == CollisionVolumeType::Sphere && volB->type == CollisionVolumeType::OBB)
	{
		collisionInfo.a = b;
		collisionInfo.b = a;
		return OBBShpereIntersection((OBBVolume&)(*volB), transformB, (SphereVolume&)(*volA), transformA, collisionInfo);
	}

    // AABB vs OBB pairs
	if (volA->type == CollisionVolumeType::AABB && volB->type == CollisionVolumeType::OBB)
	{
		return AABBOBBIntersection((AABBVolume&)(*volA), transformA, (OBBVolume&)(*volB), transformB, collisionInfo);
	}
	if (volA->type == CollisionVolumeType::OBB && volB->type == CollisionVolumeType::AABB)
	{
		collisionInfo.a = b;
		collisionInfo.b = a;
		return AABBOBBIntersection((AABBVolume&)(*volB), transformB, (OBBVolume&)(*volA), transformA, collisionInfo);
	}

}

/** Volume Intersection */

bool CollisionDetection::AABBTest(const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& halfSizeA, const glm::vec3& halfSizeB)
{
    glm::vec3 deltaPos = posA - posB;
    glm::vec3 totalSize = halfSizeA + halfSizeB;

    if (abs(deltaPos.x) < totalSize.x && abs(deltaPos.y) < totalSize.y && abs(deltaPos.z) < totalSize.z)
    {
        return true;
    }
    return false;
}

bool CollisionDetection::OBBTest(const glm::vec3& Axis, const glm::vec3& halfSizeA, const glm::vec3& rotA, const glm::vec3& halfSizeB, glm::vec3& rotB, float penetration, glm::vec3& collisionNormal)
{
    if (Axis.length() < 1e-8f) return true;
    return false;
}

bool CollisionDetection::AABBIntersection(
    const AABBVolume& volumeA, const Transform& worldTransformA, 
    const AABBVolume& volumeB, const Transform& worldTransformB, 
    CollisionInfo& collisionInfo)
{
    using namespace glm;
    vec3 posA = worldTransformA.GetPosition();
    vec3 posB = worldTransformB.GetPosition();

    vec3 sizeA = volumeA.GetHalfSizes();
    vec3 sizeB = volumeB.GetHalfSizes();

    bool overlap = AABBTest(posA, posB, sizeA, sizeB);

    if (!overlap) return false;

    static const vec3 faces[6] =
    {
        vec3(-1, 0, 0), vec3(1, 0, 0),
        vec3(0, -1, 0), vec3(0, 1, 0),
        vec3(0, 0, -1), vec3(0, 0, 1)
    };

    vec3 maxA = posA + sizeA;
    vec3 minA = posA - sizeA;
    vec3 maxB = posB + sizeB;
    vec3 minB = posB - sizeB;

    float distance[6] =
    {
        (maxB.x - minA.x),
        (maxA.x - minB.x),
        (maxB.y - minA.y),
        (maxA.y - minB.y),
		(maxB.z - minA.z),
		(maxA.z - minB.z)
    };

    float penetration = FLT_MAX;
    vec3 collisionNormal;

    for (int i = 0; i < 6; ++i)
    {
        if (distance[i] < penetration)
        {
            penetration = distance[i];
            collisionNormal = faces[i];
        }
    }

    collisionInfo.AddContactPoint(vec3(0), vec3(0), collisionNormal, penetration);
    return true;
}

bool CollisionDetection::OBBIntersection(
    const OBBVolume& volumeA, const Transform& worldTransformA, 
    const OBBVolume& volumeB, const Transform& worldTransformB, 
    CollisionInfo& collisionInfo)
{
    using namespace glm;

    vec3 sizeA = volumeA.GetHalfSizes();
    vec3 sizeB = volumeB.GetHalfSizes();


    return false;
}

bool CollisionDetection::SphereIntersection(
    const SphereVolume& volumeA, const Transform& worldTransformA, 
    const SphereVolume& volumeB, const Transform& worldTransformB, 
    CollisionInfo& collisionInfo)
{
    using namespace glm;

    float radiusA = volumeA.GetRadius();
    float radiusB = volumeB.GetRadius();

    vec3 delta = worldTransformB.GetPosition() - worldTransformA.GetPosition();
    float distance = delta.length();
    
    if (distance >= radiusA + radiusB) return false;

    float penetration = radiusA + radiusB - distance;
    vec3 collisionNormal = normalize(delta);
    vec3 localA = collisionNormal * radiusA;
    vec3 localB = -collisionNormal * radiusB;

    collisionInfo.AddContactPoint(localA, localB, collisionNormal, penetration);
    return true;
}

bool CollisionDetection::AABBShpereIntersection(
    const AABBVolume& volumeA, const Transform& worldTransformA, 
    const SphereVolume& volumeB, const Transform& worldTransformB, 
    CollisionInfo& collisionInfo)
{
    using namespace glm;
    
    vec3 sizeA = volumeA.GetHalfSizes();
    float radius = volumeB.GetRadius();

    vec3 sphereLocalPos = worldTransformB.GetPosition() - worldTransformA.GetPosition();

    vec3 closetPointOnBox = glm::clamp(sphereLocalPos, -sizeA, sizeA);

    vec3 delta = sphereLocalPos - closetPointOnBox;
    float distance = delta.length();

    if (distance >= radius) return false;

    vec3 collisionNormal = normalize(delta);
    float penetration = radius - distance;

    vec3 localA = vec3(0);
    vec3 localB = -collisionNormal * radius;

    collisionInfo.AddContactPoint(localA, localB, collisionNormal, penetration);
    return true;
} 

bool CollisionDetection::OBBShpereIntersection(const OBBVolume& volumeA, const Transform& worldTransformA, const SphereVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo)
{
    using namespace glm;
    
    vec3 sizeA = volumeA.GetHalfSizes();
    float radius = volumeB.GetRadius();

    vec3 deltaPos = worldTransformB.GetPosition() - worldTransformA.GetPosition();
    vec3 sphereLocalPos = inverse(worldTransformA.GetQuatRotation()) * deltaPos;

    vec3 closetPointOnBoxLocal = glm::clamp(sphereLocalPos, -sizeA, sizeA);
    vec3 closetPointOnBox = worldTransformA.GetQuatRotation() * closetPointOnBoxLocal;

    vec3 delta = deltaPos - closetPointOnBox;
    float distance = delta.length();

    if (distance >= radius) return false;
    vec3 collisionNormal = normalize(delta);
    float penetration = radius - distance;

    vec3 localA = closetPointOnBox;
    vec3 localB = -collisionNormal * radius;

    collisionInfo.AddContactPoint(localA, localB, collisionNormal, penetration);
    return true;
}

bool CollisionDetection::AABBOBBIntersection(const AABBVolume& volumeA, const Transform& worldTransformA, const OBBVolume& volumeB, const Transform& worldTransformB, CollisionInfo& collisionInfo)
{
    return false;
}

bool CollisionDetection::RayPlaneIntersection(const Ray& r, const Plane& plane, RayCollision& collision)
{
    using namespace glm;
    
    vec3 normal = plane.GetNormal();
    float disPlane = plane.GetDistance();

    float cosTheta = glm::dot(normal, r.GetDirection());
    if (cosTheta == 0) return false;

    vec3 rPos = r.GetPosition();
    float rProj = glm::dot(rPos, normal);
    float distance = rProj - disPlane;
    
    float rayDistance = -distance / cosTheta;
    if (rayDistance < 0) return false;            // plane is behind ray

    collision.rayDistance = rayDistance;
    collision.collidedAt = rPos + r.GetDirection() * rayDistance;
    return true;
}

bool CollisionDetection::RayTriangleIntersection(const Ray& r, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, RayCollision& collision)
{
    using namespace glm;

    const float EPSILON = 0.0000001f;

    vec3 edge1 = v1 - v0;
    vec3 edge2 = v2 - v0;

    float a, f, u, v;
    vec3 h = glm::cross(r.GetDirection(), edge2);
    a = glm::dot(edge1, h);
    if (a > -EPSILON && a < EPSILON) return false;  // ray is parallel to this triangle

    f = 1.0f / a;
    vec3 s = r.GetPosition() - v0;
    u = f * glm::dot(s, h);

    if (u < 0.0f || u > 1.0f) return false;

    vec3 q = glm::cross(s, edge1);
    v = f * glm::dot(r.GetDirection(), q);

    if (v < 0.0f || u + v > 1.0f) return false;

    float t = f * glm::dot(edge2, q);
    if (t <= EPSILON) return false;     // means that there is a line intersection but not a ray intersection.

    collision.collidedAt = r.GetPosition() + r.GetDirection() * t;
    collision.rayDistance = t;
    return true;
}

bool CollisionDetection::RayBoxIntersection(const Ray& r, const glm::vec3& boxPos, const glm::vec3& boxSize, RayCollision& collision)
{
    using namespace glm;
    
    vec3 boxMax = boxPos + boxSize;
    vec3 boxMin = boxPos - boxSize;

    vec3 rPos = r.GetPosition();
    vec3 rDir = r.GetDirection();

    vec3 tVals(-1, -1, -1);

    for (int i = 0; i < 3; ++i)     
    {
        if (rDir[i] > 0)
        {
            tVals[i] = (boxMin[i] - rPos[i]) / rDir[i];         // tVal is the rayDistance of each box side
        }
        else if (rDir[i] < 0)
        {
            tVals[i] = (boxMax[i] - rPos[i]) / rDir[i];
        }
    }

    float bestT = std::max(std::max(tVals.x, tVals.y), tVals.z);
    if (bestT < 0) return false;                                    // box is behind ray
    
    vec3 intersection = rPos + rDir * bestT;
    for (int i = 0; i < 3; ++i)
    {
        if (intersection[i] < boxMin[i] || intersection[i] > boxMax[i])
        {
            return false;               // best intersection not on box surface
        }
    }

    collision.collidedAt = intersection;
    collision.rayDistance = bestT;
    return true;
}

bool CollisionDetection::RayAABBIntersection(const Ray& r, const AABBVolume& volume, const Transform& worldTransform, RayCollision& collision)
{
    glm::vec3 boxPos = worldTransform.GetPosition();
    glm::vec3 boxSize = volume.GetHalfSizes();
    return RayBoxIntersection(r, boxPos, boxSize, collision);
}

bool CollisionDetection::RayOBBIntersection(const Ray& r, const OBBVolume& volume, const Transform& worldTransform, RayCollision& collision)
{
    using namespace glm;
    vec3 boxPos = worldTransform.GetPosition();
    quat boxRot = worldTransform.GetQuatRotation();

    vec3 localRayPos = r.GetPosition() - boxPos;
    vec3 localRayDir = glm::inverse(boxRot) * r.GetDirection();
    Ray tempRay(localRayPos, localRayDir);

    bool bIntersected = RayBoxIntersection(tempRay, vec3(), volume.GetHalfSizes(), collision);

    if (!bIntersected) return false;

    collision.collidedAt = boxRot * collision.collidedAt + boxPos;
    return true;
}

bool CollisionDetection::RaySphereIntersection(const Ray& r, const SphereVolume& volume, const Transform& worldTransform, RayCollision& collision)
{
    using namespace glm;

    vec3 rPos = r.GetPosition();
    vec3 rDir = r.GetDirection();
    vec3 spherePos = worldTransform.GetPosition();
    float radius = volume.GetRadius();

    vec3 delta = spherePos - rPos;
    float deltaProj = glm::dot(delta, rDir);

    if (deltaProj < 0.0f) return false; // Sphere is behind ray
    
    vec3 closetPoint = rPos + rDir * deltaProj;
    float distance = (spherePos - closetPoint).length();

    if (distance > radius) return false;

    float offset = sqrt(radius * radius - distance * distance);

    collision.rayDistance = deltaProj - offset;
    collision.collidedAt = rPos + rDir * (deltaProj - offset);

    return true;
}

