#pragma once

#include "Transform.h"
#include <vector>

class RenderObject;
class PhysicsObject;
class CollisionVolume;
class Model;
class Level;

class GameObject
{
public:
	GameObject(const std::string& newName, Level* currentLevel, CollisionVolume* newVolumn, Model* newModel, PhysicsObject* newPhysicsObject);
	~GameObject();

	void UpdateCollisionVolumeAABB();
	void UpdateCollisionVolumeSphere();

	Transform& GetTransform() { return transform; }
	PhysicsObject* GetPhysicsObject() const { return physicsObject; }
	const CollisionVolume* GetBoundingVolume() const { return volume; }

	Model* GetModel() { return model; }
	void SetModel(Model* val);

	float GetBoundingSphere() const { return BoundingSphere; }
	bool GetBoundingAABB(glm::vec3& size) const;

	void SetCameraDistance(float val);
	inline float GetCameraDistance() const { return distanceFromCamera; }

	inline std::string GetName() const { return name; }

	inline const std::vector<RenderObject*>& GetRenderObjects() const { return renderObjects; }

	static bool CompareByCameraDistance(const GameObject* a, const GameObject* b) { return (a->distanceFromCamera < b->distanceFromCamera) ? true : false; }

protected:
	glm::vec3 GetBoundingAABBForOBB();

protected:
	Level* level;
	Transform transform;

	unsigned int worldID;
	std::string name;

	CollisionVolume* volume = nullptr;

	Model* model;
	std::vector<RenderObject*> renderObjects;

	PhysicsObject* physicsObject = nullptr;

	glm::vec3 BoundingAABB;
	float BoundingSphere = 10.0f;

	float distanceFromCamera;
};

