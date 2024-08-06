#include "GameObject.h"
#include "RenderObject.h"
#include "CollisionVolumn.h"
#include "Model.h"

GameObject::GameObject(const std::string& newName, Level* currentLevel, CollisionVolume* newVolumn, Model* newModel, PhysicsObject* newPhysicsObject)
{
	name = newName;
	level = currentLevel;
	volume = newVolumn;
	UpdateCollisionVolumeSphere();
	physicsObject = newPhysicsObject;
	SetModel(newModel);
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
		BoundingAABB = GetBoundingAABBForOBB();
		break;

	case CollisionVolumeType::Sphere:
		float radius = static_cast<SphereVolume*>(volume)->GetRadius();
		BoundingAABB = glm::vec3(radius, radius, radius);
		break;
	}
}

void GameObject::UpdateCollisionVolumeSphere()
{
	if (volume == nullptr) return;

	switch (volume->type)
	{
	case CollisionVolumeType::AABB:
		BoundingSphere = static_cast<AABBVolume*>(volume)->GetHalfSizes().length();	//TODO: why can access private base class's type
		break;

	case CollisionVolumeType::OBB:
		BoundingSphere = static_cast<OBBVolume*>(volume)->GetHalfSizes().length();
		break;

	case CollisionVolumeType::Sphere:
		BoundingSphere = static_cast<SphereVolume*>(volume)->GetRadius();
		break;
	}
}

void GameObject::SetModel(Model* val)
{
	for (RenderObject* object : renderObjects)
	{
		delete object;
	}
	renderObjects.clear();

	model = val;
	const std::vector<Mesh*>& meshes = model->GetMeshes();
	for (auto mesh : meshes)
	{
		RenderObject* newObj = new RenderObject(level, mesh, &transform, model->GetMaterialIndices());
		renderObjects.push_back(newObj);
	}
}

bool GameObject::GetBoundingAABB(glm::vec3& size) const
{
	if (volume == nullptr) return false;

	size = BoundingAABB;
	return true;
}

void GameObject::SetCameraDistance(float val)
{
	distanceFromCamera = val;
	for (auto obj : renderObjects)
	{
		obj->SetCameraDistance(val);
	}
}

glm::vec3 GameObject::GetBoundingAABBForOBB()
{
	glm::vec3 size = static_cast<OBBVolume*>(volume)->GetHalfSizes();
	const glm::vec3 vertices[4] =
	{
		glm::vec3(size.x, size.y, size.z),
		glm::vec3(-size.x, size.y, size.z),
		glm::vec3(size.x, -size.y, size.z),
		glm::vec3(size.x, size.y, -size.z)
	};
	glm::quat rot = transform.GetQuatRotation();
	
	glm::vec3 res(0.0f);
	for (const auto& i : vertices)
	{
		res.x = abs(i.x) > res.x ? abs(i.x) : res.x;
		res.y = abs(i.y) > res.y ? abs(i.y) : res.y;
		res.z = abs(i.z) > res.z ? abs(i.z) : res.z;
	}
	return res;
}
