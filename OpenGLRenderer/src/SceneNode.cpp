#include "SceneNode.h"
#include "Model.h"
#include "mesh.h"
#include "Shader.h"

SceneNode::SceneNode(Model* m)
{
	model = m;
	mesh = nullptr;
	localTransform = Transform();
	worldTransform = Transform();
	parent = nullptr;
}

SceneNode::SceneNode(Mesh* m)
{
	model = nullptr;
	mesh = m;
	localTransform = Transform();
	worldTransform = Transform();
	parent = nullptr;
}

SceneNode::~SceneNode()
{
	for (SceneNode* child : children)
	{
		delete child;
	}
}

void SceneNode::AddChild(SceneNode* child)
{
	children.push_back(child);
	child->parent = this;
}

void SceneNode::Draw(Shader* s)
{
	if (mesh)
	{
		mesh->Draw(*s);
	}
	if (model)
	{
		model->Draw(*s);
	}
}

void SceneNode::Update(float dt)
{
	if (parent)
	{
		worldTransform = parent->GetWorldTransform() * localTransform;
	}
	else
	{
		worldTransform = localTransform;
	}
	for (SceneNode* child : children)
	{
		child->Update(dt);
	}
}
