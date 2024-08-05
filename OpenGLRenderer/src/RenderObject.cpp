#include "RenderObject.h"
#include "Model.h"

RenderObject::RenderObject(Model* newModel, Transform* parentTransform)
{
	model = newModel;
	transform = parentTransform;
}

RenderObject::~RenderObject()
{
}
