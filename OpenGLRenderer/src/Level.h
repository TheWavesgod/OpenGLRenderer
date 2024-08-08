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

	GameObject* AddGameObject(const std::string& name, const glm::vec3& position, CollisionVolume* volume, Model* model);

	LightsManager* GetLightsManager() { return lightsManager; }
	std::vector<Material*>& GetMaterials() { return  materials; }
	std::vector<Model*>& GetModels() { return models; }
	std::vector<GameObject*>& GetGameObjects() { return gameObjects; }
		
	Material* GetMaterialByIndex(int i);

	Model* GenerateSphereModel(const std::string& name);
	Model* GenerateCubeModel(const std::string& name);
	Model* GenerateFloorModel(const std::string& name);

	/** Setting parameter */
	int selectSkybox = 0;

protected:
	void ConstructScene();
	void ImportCubeMaps();

	void Render();
	void DrawScene();

	void BuildNodeLists(SceneNode* from);

	void BuildRenderList();
	void SortRenderList();
	void ClearRenderList();
	void DrawRenderList();

	void CheckSelectSkybox();

	Camera* camera;
	SceneNode* root;
	LightsManager* lightsManager;

	int currentSkybox = 0;
	std::vector<CubeMap*> CubeMaps;

	std::vector<Mesh*> meshes;

	std::vector<Material*> materials;
	std::vector<Model*> models;

	glRenderer* renderer;
	Frustum frameFrustum;

	std::vector<RenderObject*> transparentRenderList;
	std::vector<RenderObject*> opaqueRenderList;

	std::vector<GameObject*> gameObjects;
};

