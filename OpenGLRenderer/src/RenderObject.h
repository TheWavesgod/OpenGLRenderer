#pragma once
#include <vector>

class Transform;
class CollisionVolume;
class Mesh;
class Material;
class Level;

class RenderObject
{
public:
	RenderObject(Level* newLevel, Mesh* newMesh, Transform* parentTransform, std::vector<int>* materials);
	~RenderObject();

	void Draw();

	inline void SetCameraDistance(float val) { distanceFromCamera = val; }
	inline float GetCameraDistance() const { return distanceFromCamera; }
	
	const Transform& GetTrasform() const { return *transform; }

	void SetLevel(Level* newLevel) { level = newLevel; }

	bool GetIsTransparentObject() const;

	static bool CompareByCameraDistance(const RenderObject* a, const RenderObject* b) { return (a->distanceFromCamera < b->distanceFromCamera) ? true : false; }

protected:
	Transform* transform = nullptr;
	Mesh* mesh;
	Material* material;
	std::vector<int>* parentMaterialIndices;
	Level* level;

	float distanceFromCamera = 0.0f;
private:

};