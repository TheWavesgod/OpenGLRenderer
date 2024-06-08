#pragma once

#include "Transform.h"

#include <vector>

class Model;
class Mesh;
class Shader;

class SceneNode
{

public:
	SceneNode();
	SceneNode(Model* m);
	SceneNode(Mesh* m);
	~SceneNode();

	void AddChild(SceneNode* sn);

	virtual void Draw();
	virtual void Update(float dt);

	void DrawToLightDepthMap(Shader* shader);

	static bool CompareByCameraDistance(const SceneNode* a, const SceneNode* b) { return (a->distanceFromCamera < b->distanceFromCamera) ? true : false; }

protected:
	SceneNode* parent;

	Model* model;
	Mesh* mesh;

	Transform worldTransform;
	Transform localTransform;

	std::vector<SceneNode*> children;

	float distanceFromCamera = 0.0f;
	float boundingRadius = 1.0f;
	bool bIsTransparent = false;

public:
	inline void SetWorldTransform(const Transform& val) { worldTransform = val; }
	inline Transform& GetWorldTransform() { return worldTransform; }

	inline void SetTransform(const Transform& val) { localTransform = val; }
	inline Transform& GetTransform() { return localTransform; }

	inline void SetModel(Model* val) { model = val; }
	inline Model* GetModel() const { return model; }

	inline void SetMesh(Mesh* val) { mesh = val; }
	inline Mesh* GetMesh() const { return mesh; }

	inline void SetBoundingRadius(float val) { boundingRadius = val; }
	inline float GetBoundingRadius() const { return boundingRadius; }

	inline void SetCameraDistance(float val) { distanceFromCamera = val; }
	inline float GetCameraDistance() const { return distanceFromCamera; }

	inline void SetIsTransparent(bool val) { bIsTransparent = val; }
	inline bool GetIsTransparent() const { return bIsTransparent; }

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart() { return children.begin(); }
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd() { return children.end(); }
};

