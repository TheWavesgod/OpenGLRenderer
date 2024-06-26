#pragma once

#include "Frustum.h"

#include <vector>

class SceneNode;
class Camera;
class CubeMap;
class Mesh;
class Texture;
class Material;
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
	CubeMap* skybox1;

	Mesh* floor;
	Mesh* cube;
	Mesh* RustedIron;
	Mesh* BrickWall;
	Mesh* GrassMeadow;
	Mesh* StainlessSteel;
	Mesh* WhiteMarble;
	Mesh* Lava;

	std::vector<Material*> materials;

	Model* backpack;

	glRenderer* renderer;
	Frustum frameFrustum;

	std::vector<SceneNode*> transparentNodeList;
	std::vector<SceneNode*> nodeList;
};

