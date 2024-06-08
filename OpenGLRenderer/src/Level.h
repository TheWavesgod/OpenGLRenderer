#pragma once

#include "Frustum.h"

#include <vector>

class SceneNode;
class Camera;
class CubeMap;
class Mesh;
class Texture;
class glRenderer;
class LightsManager;
class Model;

class Level
{
public:
	Level(glRenderer* r);
	~Level();

	void Update(float dt);

	void LevelBeginPlay();

protected:
	void ConstructScene();

	void Render();
	void DrawScene();

	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);

	Camera* camera;
	SceneNode* root;
	LightsManager* lightsManager;
	CubeMap* skybox;

	Mesh* floor;
	Mesh* cube;

	Model* backpack;

	glRenderer* renderer;
	Frustum frameFrustum;

	std::vector<SceneNode*> transparentNodeList;
	std::vector<SceneNode*> nodeList;
};

