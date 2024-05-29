#pragma once

#include "Transform.h"

#include <vector>

class Model;
class Mesh;
class Shader;

class SceneNode
{

public:
	SceneNode(Model* m = nullptr);
	SceneNode(Mesh* m = nullptr);
	~SceneNode();

	void AddChild(SceneNode* sn);

	virtual void Draw(Shader* s);
	virtual void Update(float dt);

private:
	SceneNode* parent;

	Model* model;
	Mesh* mesh;

	Transform worldTransform;
	Transform localTransform;

	std::vector<SceneNode*> children;

public:
	inline void SetWorldTransform(const Transform& val) { worldTransform = val; }
	inline const Transform& GetWorldTransform() const { return worldTransform; }

	inline void SetLocalTransform(const Transform& val) { localTransform = val; }
	inline const Transform& GetLocalTransform() const { return localTransform; }

	inline void SetModel(Model* val) { model = val; }
	inline Model* GetModel() const { return model; }

	inline void SetMesh(Mesh* val) { mesh = val; }
	inline Mesh* GetMesh() const { return mesh; }

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart() { return children.begin(); }
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd() { return children.end(); }
};

