#pragma once

#include "Frustum.h"
#include "PhysicsSystem.h"

#include <vector>

class GameObject;
class SceneNode;
class Camera;
class CubeMap;
class Mesh;
class Texture;
class Material;
class glRenderer;
class LightsManager;
class Model;
class RenderObject;

using GameObjectIterator = std::vector<GameObject*>::const_iterator;

class Level
{
public:
	Level(glRenderer* r);
	~Level();

	void Update(float dt);

	void LevelBeginPlay();

	void GetGameObjectsIterators(GameObjectIterator& cbegin, GameObjectIterator& cend) const;

	bool Raycast(Ray& r, RayCollision& collision, bool closestObject = false, GameObject* ignore = nullptr) const;

	LightsManager* GetLightsManager() { return lightsManager; }
	std::vector<Material*>& GetMaterials() { return  materials; }

	/** Setting parameter */
	int selectSkybox = 0;

protected:
	void ConstructScene();
	void ImportCubeMaps();

	void Render();
	void DrawScene();

	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);

	void BuildRenderObjectLists();
	void SortRenderObjectLists();
	void ClearRenderObjectLists();

	void CheckSelectSkybox();

	Camera* camera;
	SceneNode* root;
	LightsManager* lightsManager;

	int currentSkybox = 0;
	std::vector<CubeMap*> CubeMaps;

	Mesh* floor;
	Mesh* cube;
	Mesh* RustedIron;
	Mesh* BrickWall;
	Mesh* GrassMeadow;
	Mesh* StainlessSteel;
	Mesh* WhiteMarble;
	Mesh* Lava;

	std::vector<Material*> materials;
	std::vector<Model*> models;
	Model* backpack;
	Model* F22;

	glRenderer* renderer;
	Frustum frameFrustum;

	std::vector<SceneNode*> transparentNodeList;
	std::vector<SceneNode*> nodeList;

	std::vector<RenderObject*> transparentObjectList;
	std::vector<RenderObject*> objectList;

	std::vector<GameObject*> gameObjects;
};

