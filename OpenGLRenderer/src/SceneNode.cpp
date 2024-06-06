#include "SceneNode.h"
#include "Model.h"
#include "mesh.h"
#include "Shader.h"

SceneNode::SceneNode()
{
	model = nullptr;
	mesh = nullptr;
	localTransform = Transform();
	worldTransform = Transform();
	parent = nullptr;
}

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

void SceneNode::Draw()
{
	if (mesh)
	{
		Shader* useShader = Shader::GetShaderByIndex(mesh->shaderIndex);
		useShader->Use();
		useShader->SetUniformMat4("model", worldTransform.GetTransMatrix());

		mesh->Draw(*useShader);
	}

	if (model)
	{
		Shader* useShader = Shader::GetShaderByIndex(model->shaderIndex);
		useShader->Use();
		useShader->SetUniformMat4("model", worldTransform.GetTransMatrix());
		model->Draw(*useShader);
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
