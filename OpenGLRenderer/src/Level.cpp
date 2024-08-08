#include "Level.h"
#include "SceneNode.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "glRenderer.h"
#include "Light.h"
#include "Shader.h"
#include "Model.h"
#include "GameObject.h"
#include "RenderObject.h"

#include <algorithm>

Level::Level(glRenderer* r)
{
	renderer = r;

	camera = new Camera(0.0f, 0.0f, glm::vec3(0.0f, 2.0f, 3.0f));

	lightsManager = new LightsManager();

	ImportCubeMaps();

	Material* newMaterial = new Material(
		"../Resources/Textures/RustedIron/rustediron2_basecolor.png",
		"../Resources/Textures/RustedIron/rustediron2_metallic.png",
		"../Resources/Textures/RustedIron/rustediron2_roughness.png",
		"../Resources/Textures/RustedIron/rustediron2_normal.png",
		"",
		"../Resources/Textures/RustedIron/rustediron2_ao.png"
	);
	newMaterial->SetName("RustedIron");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Textures/BrickWall/brick-wall_albedo.png",
		"../Resources/Textures/BrickWall/brick-wall_metallic.png",
		"../Resources/Textures/BrickWall/brick-wall_roughness.png",
		"../Resources/Textures/BrickWall/brick-wall_normal-dx.png",
		"../Resources/Textures/BrickWall/brick-wall_height.png",
		"../Resources/Textures/BrickWall/brick-wall_ao.png"
	);
	newMaterial->SetName("BrickWall");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Textures/GrassMeadow/grass-meadow_albedo.png",
		"../Resources/Textures/GrassMeadow/grass-meadow_metallic.png",
		"../Resources/Textures/GrassMeadow/grass-meadow_roughness.png",
		"../Resources/Textures/GrassMeadow/grass-meadow_normal-dx.png",
		"../Resources/Textures/GrassMeadow/grass-meadow_height.png",
		"../Resources/Textures/GrassMeadow/grass-meadow_ao.png"
	);
	newMaterial->SetName("GrassMeadow");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Textures/StainlessSteel/used-stainless-steel2_albedo.png",
		"../Resources/Textures/StainlessSteel/used-stainless-steel2_metallic.png",
		"../Resources/Textures/StainlessSteel/used-stainless-steel2_roughness.png",
		"../Resources/Textures/StainlessSteel/used-stainless-steel2_normal-dx.png",
		"../Resources/Textures/StainlessSteel/used-stainless-steel2_height.png",
		"../Resources/Textures/StainlessSteel/used-stainless-steel2_ao.png"
	);
	newMaterial->SetName("UsedStainlessSteel");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Textures/WhiteMarble/white-marble_albedo.png",
		"../Resources/Textures/WhiteMarble/white-marble_metallic.png",
		"../Resources/Textures/WhiteMarble/white-marble_roughness.png",
		"../Resources/Textures/WhiteMarble/white-marble_normal-dx.png",
		"../Resources/Textures/WhiteMarble/white-marble_height.png",
		"../Resources/Textures/WhiteMarble/white-marble_ao.png"
	);
	newMaterial->SetName("WhiteMarble");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Textures/ColumnedLavaRock/columned-lava-rock_albedo.png",
		"../Resources/Textures/ColumnedLavaRock/columned-lava-rock_metallic.png",
		"../Resources/Textures/ColumnedLavaRock/columned-lava-rock_roughness.png",
		"../Resources/Textures/ColumnedLavaRock/columned-lava-rock_normal-dx.png",
		"../Resources/Textures/ColumnedLavaRock/columned-lava-rock_height.png",
		"../Resources/Textures/ColumnedLavaRock/columned-lava-rock_ao.png",
		"../Resources/Textures/ColumnedLavaRock/columned-lava-rock_emissive.png"
	);
	newMaterial->SetName("LavaRock");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Textures/HardwoodPlanks/HardwoodPlanks_albedo.png",
		"../Resources/Textures/HardwoodPlanks/HardwoodPlanks_metallic.png",
		"../Resources/Textures/HardwoodPlanks/HardwoodPlanks_roughness.png",
		"../Resources/Textures/HardwoodPlanks/HardwoodPlanks_normal-dx.png",
		"../Resources/Textures/HardwoodPlanks/HardwoodPlanks_height.png",
		"../Resources/Textures/HardwoodPlanks/HardwoodPlanks_ao.png"
	);
	newMaterial->SetName("HardwoodPlanks");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/backpack/albedo.jpg",
		"../Resources/Models/backpack/metallic.jpg",
		"../Resources/Models/backpack/roughness.jpg",
		"../Resources/Models/backpack/normal.png",
		"",
		"../Resources/Models/backpack/ao.jpg"
	);
	newMaterial->SetName("Backpack");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/F22/exterior_diffuse.png",
		"../Resources/Models/F22/exterior_reflection.png",
		"../Resources/Models/F22/exterior_specular.png",
		"../Resources/Models/F22/exterior_normal.png",
		"",
		""
	);
	newMaterial->SetName("F22 exterior");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/F22/interior_diffuse.png",
		"../Resources/Models/F22/interior_reflection.png",
		"../Resources/Models/F22/interior_specular.png",
		"../Resources/Models/F22/interior_normal.png",
		"",
		""
	);
	newMaterial->SetName("F22 interior");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/F22/pilot_diffuse.png",
		"../Resources/Models/F22/pilot_specular.png",
		"../Resources/Models/F22/pilot_specular.png",
		"../Resources/Models/F22/pilot_normal.png",
		"",
		""
	);
	newMaterial->SetName("F22 pilot");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/Seagull/Seagull_cockpit_BaseColor.jpg",
		"",
		"../Resources/Models/Seagull/Seagull_cockpit_Roughness.jpg",
		"../Resources/Models/Seagull/Seagull_cockpit_Normal.jpg",
		"",
		""
	);
	newMaterial->SetName("Seagull cockpit");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/Seagull/Seagull_control_panel_BaseColor.jpg",
		"",
		"../Resources/Models/Seagull/Seagull_control_panel_Roughness.jpg",
		"../Resources/Models/Seagull/Seagull_control_panel_Normal.jpg",
		"",
		"",
		"../Resources/Models/Seagull/Seagull_control_panel_Emissive.jpg"
	);
	newMaterial->SetName("Seagull control panel");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/Seagull/Seagull_Hanger_BaseColor.jpg",
		"",
		"../Resources/Models/Seagull/Seagull_Hanger_Roughness.jpg",
		"../Resources/Models/Seagull/Seagull_Hanger_Normal.jpg",
		"",
		""
	);
	newMaterial->SetName("Seagull Hanger");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/Seagull/Seagull_Ladder_BaseColor.jpg",
		"",
		"../Resources/Models/Seagull/Seagull_Ladder_Roughness.jpg",
		"../Resources/Models/Seagull/Seagull_Ladder_Normal.jpg",
		"",
		""
	);
	newMaterial->SetName("Seagull Ladder");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/Seagull/Seagull_mainbody_BaseColor.jpg",
		"",
		"../Resources/Models/Seagull/Seagull_mainbody_Roughness.jpg",
		"../Resources/Models/Seagull/Seagull_mainbody_Normal.jpg",
		"",
		""
	);
	newMaterial->SetName("Seagull MainBody");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/Seagull/Seagull_Missile_BaseColor.jpg",
		"",
		"../Resources/Models/Seagull/Seagull_Missile_Roughness.jpg",
		"../Resources/Models/Seagull/Seagull_Missile_Normal.jpg",
		"",
		""
	);
	newMaterial->SetName("Seagull Missile");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/Seagull/Seagull_Pilot_seat_BaseColor.jpg",
		"",
		"../Resources/Models/Seagull/Seagull_Pilot_seat_Roughness.jpg",
		"../Resources/Models/Seagull/Seagull_Pilot_seat_Normal.jpg",
		"",
		""
	);
	newMaterial->SetName("Seagull Seat");
	materials.push_back(newMaterial);

	newMaterial = new Material(
		"../Resources/Models/Seagull/Seagull_Wings_BaseColor.jpg",
		"",
		"../Resources/Models/Seagull/Seagull_Wings_Roughness.jpg",
		"../Resources/Models/Seagull/Seagull_Wings_Normal.jpg",
		"",
		""
	);
	newMaterial->SetName("Seagull Wings");
	materials.push_back(newMaterial);

	newMaterial = Material::CreateGlassMaterial("F22 Glass", glm::vec3(1.0f, 0.324, 0.0f), 0.5f, 0.043f, 0.0f);
	materials.push_back(newMaterial);

	newMaterial = Material::CreateGlassMaterial("Seagull Glass", glm::vec3(0.006f, 0.035f, 0.103f), 0.5f, 0.703f, 0.338f);
	materials.push_back(newMaterial);

	newMaterial = new Material();
	newMaterial->SetName("PBRExample");
	materials.push_back(newMaterial);

	meshes.emplace_back(Mesh::GenerateFloor());
	meshes.emplace_back(Mesh::GenerateCube());
	meshes.emplace_back(Mesh::GenerateSphere());

	Model* backpack = new Model("backpack", "../Resources/Models/backpack/backpack.obj");
	models.push_back(backpack);
	backpack->SetMaterialIndex(0, 7);
	backpack->SetMaterialIndex(1, 7);

	Model* F22 = new Model("F22", "../Resources/Models/F22/F22.obj", true);
	models.push_back(F22);
	F22->SetMaterialIndex(0, 0);
	F22->SetMaterialIndex(1, 10);
	F22->SetMaterialIndex(2, 8);
	F22->SetMaterialIndex(3, 19);
	F22->SetMaterialIndex(4, 9);

	Model* seagull = new Model("Seagull", "../Resources/Models/Seagull/Seagull.obj", true);
	models.push_back(seagull);
	seagull->SetMaterialIndex(0, 0);
	seagull->SetMaterialIndex(1, 0);
	seagull->SetMaterialIndex(2, 20);
	seagull->SetMaterialIndex(3, 13);
	seagull->SetMaterialIndex(4, 15);
	seagull->SetMaterialIndex(5, 16);
	seagull->SetMaterialIndex(6, 17);
	seagull->SetMaterialIndex(7, 11);
	seagull->SetMaterialIndex(8, 12);
	seagull->SetMaterialIndex(9, 14);
	seagull->SetMaterialIndex(10, 18);

	ConstructScene();

	LevelBeginPlay();
}

Level::~Level()
{
	delete camera;
	delete lightsManager;
}

void Level::ConstructScene()
{
	lightsManager->AddDirectionalLight(glm::vec3(-80.0f, 0.0f, 0.0f), glm::vec3(5.0f));
	lightsManager->AddSpotLight(glm::vec3(3.0f, 5.0f, 0.0f), glm::vec3(-60.0f, 0.0f, 0.0f), glm::vec3(1.2f, 7.0f, 0.0f), 30.0f, 35.0f, 0.07f, 0.017f);
	lightsManager->AddSpotLight(glm::vec3(6.0f, 5.0f, 28.0f), glm::vec3(-30.0f, 45.0f, 0.0f), glm::vec3(9.2f, 8.9f, 4.2f), 30.0f, 35.0f, 0.07f, 0.017f);
	lightsManager->AddSpotLight(glm::vec3(-6.0f, 5.0f, 28.0f), glm::vec3(-30.0f, -45.0f, 0.0f), glm::vec3(9.2f, 8.9f, 4.2f), 30.0f, 35.0f, 0.07f, 0.017f);
	lightsManager->AddPointLight(glm::vec3(-3.0f, 4.5f, 1.0f), glm::vec3(3.0f, 0.0f, 1.5f), 0.09f, 0.032f);

	GameObject* newObj;
	newObj = AddGameObject("Floor", glm::vec3(0.0f), new AABBVolume(glm::vec3(5.0f, 5.0f, 5.0f)), GenerateFloorModel("Floor"));
	newObj->GetModel()->SetMaterialIndex(0, 6);
	newObj = AddGameObject("RustedIron", glm::vec3(2.0f, 1.5f, -3.0f), new AABBVolume(glm::vec3(1.0f)), GenerateCubeModel("RustedIron"));
	newObj->GetModel()->SetMaterialIndex(0, 0);
	newObj = AddGameObject("BrickWall", glm::vec3(0.0f, 1.5f, -3.0f), new AABBVolume(glm::vec3(1.0f)), GenerateCubeModel("BrickWall"));
	newObj->GetModel()->SetMaterialIndex(0, 1);
	newObj = AddGameObject("Lava", glm::vec3(-2.0f, 1.5f, -3.0f), new AABBVolume(glm::vec3(1.0f)), GenerateCubeModel("Lava"));
	newObj->GetModel()->SetMaterialIndex(0, 2);
	newObj = AddGameObject("StainlessSteel", glm::vec3(-4.0f, 1.5f, -3.0f), new AABBVolume(glm::vec3(1.0f)), GenerateCubeModel("StainlessSteel"));
	newObj->GetModel()->SetMaterialIndex(0, 3);
	newObj = AddGameObject("WhiteMarble", glm::vec3(-6.0f, 1.5f, -3.0f), new AABBVolume(glm::vec3(1.0f)), GenerateCubeModel("WhiteMarble"));
	newObj->GetModel()->SetMaterialIndex(0, 4);
	newObj = AddGameObject("GrassMeadow", glm::vec3(-8.0f, 1.5f, -3.0f), new AABBVolume(glm::vec3(1.0f)), GenerateCubeModel("GrassMeadow"));
	newObj->GetModel()->SetMaterialIndex(0, 5);
	newObj = AddGameObject("PBRExample", glm::vec3( 0.0f, 5.0f, 5.0f), new SphereVolume(1.0f), GenerateSphereModel("PBRExample"));
	newObj->GetModel()->SetMaterialIndex(0, 21);


	AddGameObject("Backpack", glm::vec3(6.0f, 3.0f, -3.0f), new AABBVolume(glm::vec3(5.0f, 5.0f, 5.0f)), models[0]);
	AddGameObject("F22", glm::vec3(6.0f, 10.0f, -3.0f), new AABBVolume(glm::vec3(15.0f, 5.0f, 9.0f)), models[1]);
	AddGameObject("Seagull", glm::vec3(0.0f, 0.05f, 20.0f), new AABBVolume(glm::vec3(15.0f, 5.0f, 9.0f)), models[2]);
}

void Level::ImportCubeMaps()
{
	CubeMaps.reserve(4);
	CubeMaps[0] = new CubeMap("../Resources/HDR/RockHill/RockHill.hdr");
	CubeMaps[1] = new CubeMap("../Resources/HDR/LivingRoom/LivingRoom.hdr");
	CubeMaps[2] = new CubeMap("../Resources/HDR/DarkPlace/DarkPlace.hdr");
	CubeMaps[3] = new CubeMap("../Resources/HDR/Gallery/gallery_hd.hdr");
}

void Level::Update(float dt)
{
	camera->UpdateCamera(dt);
	camera->UploadViewMatrix(renderer->GetUboCamera());
	camera->UploadViewPos(renderer->GetUboCamera());

	Render();
}

void Level::LevelBeginPlay()
{
	lightsManager->Init();

	camera->UploadProjectionMatrix(renderer->GetUboCamera());

	CubeMaps[currentSkybox]->BindIrradianceMap(7);
	CubeMaps[currentSkybox]->BindprefilterMap(8);
	CubeMaps[currentSkybox]->BindBRDFLUT(9);
}

void Level::GetGameObjectsIterators(GameObjectIterator& cbegin, GameObjectIterator& cend) const
{
	cbegin = gameObjects.cbegin();
	cend = gameObjects.cend();
}

bool Level::Raycast(Ray& r, RayCollision& collision, bool closestObject, GameObject* ignore) const
{
	RayCollision closestCollision;
	for (auto& i : gameObjects)
	{
		if (i->GetBoundingVolume() == nullptr) continue;
		if (i == ignore) continue;

		RayCollision thisCollision;
		if (CollisionDetection::RayIntersection(r, *i, thisCollision))
		{
			if (!closestObject)
			{
				collision = thisCollision;
				collision.node = i;
				return true;
			}
			else
			{
				if (thisCollision.rayDistance < closestCollision.rayDistance)
				{
					closestCollision = thisCollision;
					closestCollision.node = i;
				}
			}
		}
	}
	if (closestCollision.node == nullptr) return false;
	collision = closestCollision;
	return true;
}

GameObject* Level::AddGameObject(const std::string& name, const glm::vec3& position, CollisionVolume* volume, Model* model)
{
	GameObject* newObj = new GameObject(name, this, volume, model, nullptr);
	newObj->GetTransform().SetPosition(position);
	gameObjects.push_back(newObj);
	return newObj;
}

Material* Level::GetMaterialByIndex(int i)
{
	if (i >= materials.size()) return materials[0];

	return materials[i];
}

Model* Level::GenerateSphereModel(const std::string& name)
{
	Model* newModel = new Model(name);
	newModel->GetMeshes().push_back(meshes[2]);

	models.push_back(newModel);
	return newModel;
}

Model* Level::GenerateCubeModel(const std::string& name)
{
	Model* newModel = new Model(name);
	newModel->GetMeshes().push_back(meshes[1]);

	models.push_back(newModel);
	return newModel;
}

Model* Level::GenerateFloorModel(const std::string& name)
{
	Model* newModel = new Model(name);
	newModel->GetMeshes().push_back(meshes[0]);

	models.push_back(newModel);
	return newModel;
}

void Level::Render()
{
	lightsManager->DrawLightDepthMaps(gameObjects);
	lightsManager->Update();
	renderer->SetSceneBufferReady();	
	lightsManager->DrawLightCubes();
	CheckSelectSkybox();
	DrawScene();
	renderer->MultiSample();
	renderer->PostProcess();
}

void Level::DrawScene()
{
	BuildRenderList();
	SortRenderList();
	DrawRenderList();
	ClearRenderList();

	// Draw skybox last
	CubeMaps[currentSkybox]->Draw();
}

void Level::BuildNodeLists(SceneNode* from)
{
	if (from == nullptr) return;

	if (frameFrustum.InsideFrustum(*from))
	{
		glm::vec3 dir = from->GetWorldTransform().GetPosition() - camera->GetCameraTransform().GetPosition();
		from->SetCameraDistance(glm::dot(dir, dir));

		if (from->GetIsTransparent())
		{
			
		}
		else
		{
			
		}
	}

	for (std::vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i)
	{
		BuildNodeLists(*i);
	}
}

void Level::BuildRenderList()
{
	for (auto object : gameObjects)
	{
		if (object->GetModel() == nullptr) continue;
		if (1 /*frameFrustum.InsideFrustum(object)*/)
		{
			glm::vec3 dir = object->GetTransform().GetPosition() - camera->GetCameraTransform().GetPosition();
			object->SetCameraDistance(glm::dot(dir, dir));
			
			auto& renderObjects = object->GetRenderObjects();
			for (RenderObject* obj : renderObjects)
			{
				if (obj->GetIsTransparentObject())
				{
					transparentRenderList.push_back(obj);
				}
				else
				{
					opaqueRenderList.push_back(obj);
				}
			}
		}
	}
}

void Level::SortRenderList()
{
	std::sort(transparentRenderList.rbegin(), transparentRenderList.rend(), RenderObject::CompareByCameraDistance);
	std::sort(opaqueRenderList.begin(), opaqueRenderList.end(), RenderObject::CompareByCameraDistance);
}

void Level::ClearRenderList()
{
	transparentRenderList.clear();
	opaqueRenderList.clear();
}

void Level::DrawRenderList()
{
	for (const auto& i : opaqueRenderList)
	{
		i->Draw();
	}

	for (const auto& i : transparentRenderList)
	{
		i->Draw();
	}
}

void Level::CheckSelectSkybox()
{
	if (selectSkybox == currentSkybox) return;
	
	currentSkybox = selectSkybox;

	CubeMaps[currentSkybox]->BindIrradianceMap(7);
	CubeMaps[currentSkybox]->BindprefilterMap(8);
	CubeMaps[currentSkybox]->BindBRDFLUT(9);
}
