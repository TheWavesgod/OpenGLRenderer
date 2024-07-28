#include "CollisionDetection.h"

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
