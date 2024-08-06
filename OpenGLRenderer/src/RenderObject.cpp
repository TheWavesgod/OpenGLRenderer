#include "RenderObject.h"
#include "Model.h"
#include "mesh.h"
#include "Material.h"
#include "Level.h"
#include "Shader.h"

RenderObject::RenderObject(Level* newLevel, Mesh* newMesh, Transform* parentTransform, std::vector<int>* materials)
{
	level = newLevel;
	mesh = newMesh;
	transform = parentTransform;
	parentMaterialIndices = materials;
}

RenderObject::~RenderObject()
{
}

void RenderObject::Draw()
{
	int mIndex = (*parentMaterialIndices)[mesh->parentMaterialIndex];
	material = level->GetMaterialByIndex(mIndex);

	Shader* useShader = Shader::GetShaderByIndex(material->GetUseShaderIndex());

	useShader->Use();
	useShader->SetUniformMat4("model", transform->GetTransMatrix());

	material->Set(*useShader);
	mesh->Draw();
}

bool RenderObject::GetIsTransparentObject() const
{
	return material ? material->GetTransparent() : false;
}
