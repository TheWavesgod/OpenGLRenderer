#pragma once

class Transform;
class CollisionVolume;
class Model;

class RenderObject
{
public:
	RenderObject(Model* newModel, Transform* parentTransform);
	~RenderObject();

	inline float GetBoundingSphere() const { return boundingSphere; }
	void SetBoundingSphere(float radius) { boundingSphere = radius; }

	inline void SetCameraDistance(float val) { distanceFromCamera = val; }
	inline float GetCameraDistance() const { return distanceFromCamera; }
	
	const Transform& GetTrasform() const { return *transform; }

	static bool CompareByCameraDistance(const RenderObject* a, const RenderObject* b) { return (a->distanceFromCamera < b->distanceFromCamera) ? true : false; }

protected:
	Transform* transform = nullptr;
	Model* model;

	float boundingSphere = 1.0f;
	float distanceFromCamera = 0.0f;
private:

};