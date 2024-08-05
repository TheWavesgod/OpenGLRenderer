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
#include "RenderObject.h"

#include <algorithm>

Level::Level(glRenderer* r)
{
	renderer = r;

	camera = new Camera(0.0f, 0.0f, glm::vec3(0.0f, 2.0f, 3.0f));

	lightsManager = new LightsManager();

	/*skybox = new CubeMap(
		"../Resources/CubeMap/evening_right.jpg",
		"../Resources/CubeMap/evening_left.jpg",
		"../Resources/CubeMap/evening_top.jpg",
		"../Resources/CubeMap/evening_bottom.jpg",
		"../Resources/CubeMap/evening_front.jpg",
		"../Resources/CubeMap/evening_back.jpg"
	);*/
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

	floor = Mesh::GenerateFloor();
	floor->shaderIndex = 6;
	floor->SetMaterial(materials[6]);

	cube = Mesh::GenerateCube();
	cube->shaderIndex = 1;
	cube->AddTexture(Texture("../Resources/Textures/container2.png"));
	cube->AddTexture(Texture("../Resources/Textures/container2_specular.png", TEXTYPE_SPECULAR));

	RustedIron = Mesh::GenerateCube();
	RustedIron->shaderIndex = 6;
	RustedIron->SetMaterial(materials[0]);

	BrickWall = Mesh::GenerateCube();
	BrickWall->shaderIndex = 6;
	BrickWall->SetMaterial(materials[1]);
	
	GrassMeadow = Mesh::GenerateCube();
	GrassMeadow->shaderIndex = 6;
	GrassMeadow->SetMaterial(materials[2]);

	StainlessSteel = Mesh::GenerateCube();
	StainlessSteel->shaderIndex = 6;
	StainlessSteel->SetMaterial(materials[3]);

	WhiteMarble = Mesh::GenerateCube();
	WhiteMarble->shaderIndex = 6;
	WhiteMarble->SetMaterial(materials[4]);

	Lava = Mesh::GenerateCube();
	Lava->shaderIndex = 6;
	Lava->SetMaterial(materials[5]);

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

	backpack = new Model("../Resources/Models/backpack/backpack.obj");
	backpack->SetShaderIndex(6);
	backpack->SetMaterial(materials[7]);

	F22 = new Model("../Resources/Models/F22/F22.obj");
	F22->SetShaderIndex(6);
	F22->SetMaterial(materials[4]);

	root = new SceneNode();

	ConstructScene();

	LevelBeginPlay();
}

Level::~Level()
{
	delete camera;
	delete lightsManager;
	delete cube;
	delete RustedIron;
	delete BrickWall;
	delete GrassMeadow;
	delete StainlessSteel;
	delete WhiteMarble;
	delete Lava;
	delete root;
}

void Level::ConstructScene()
{
	//lightsManager->AddDirectionalLight(glm::vec3(-80.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	lightsManager->AddSpotLight(glm::vec3(3.0f, 5.0f, 0.0f), glm::vec3(-60.0f, 0.0f, 0.0f), glm::vec3(1.2f, 7.0f, 0.0f), 30.0f, 35.0f, 0.07f, 0.017f);
	lightsManager->AddPointLight(glm::vec3(-3.0f, 4.5f, 1.0f), glm::vec3(3.0f, 0.0f, 1.5f), 0.09f, 0.032f);

	SceneNode* Floor = new SceneNode(floor);
	root->AddChild(Floor);
	Floor->GetTransform().SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));

	/*SceneNode* container = new SceneNode(cube);
	root->AddChild(container);
	container->GetTransform().SetPosition(glm::vec3( 4.0f, 1.5f, -3.0f));*/

	SceneNode* backpackModel = new SceneNode(backpack);
	root->AddChild(backpackModel);
	backpackModel->GetTransform().SetPosition(glm::vec3(6.0f, 1.5f, -3.0f));

	SceneNode* F22Model = new SceneNode(F22);
	root->AddChild(F22Model);
	F22Model->GetTransform().SetPosition(glm::vec3(6.0f, 10.0f, -3.0f));

	SceneNode* PBRcontainer = new SceneNode(RustedIron);
	root->AddChild(PBRcontainer);
	PBRcontainer->GetTransform().SetPosition(glm::vec3( 2.0f, 1.5f, -3.0f));

	SceneNode* PBRBrickWall = new SceneNode(BrickWall);
	root->AddChild(PBRBrickWall);
	PBRBrickWall->GetTransform().SetPosition(glm::vec3( 0.0f, 1.5f, -3.0f));

	SceneNode* PBRLava = new SceneNode(Lava);
	root->AddChild(PBRLava);
	PBRLava->GetTransform().SetPosition(glm::vec3(-2.0f, 1.5f, -3.0f));

	SceneNode* PBRStainlessSteel = new SceneNode(StainlessSteel);
	root->AddChild(PBRStainlessSteel);
	PBRStainlessSteel->GetTransform().SetPosition(glm::vec3(-4.0f, 1.5f, -3.0f));

	SceneNode* PBRWhiteMarble = new SceneNode(WhiteMarble);
	root->AddChild(PBRWhiteMarble);
	PBRWhiteMarble->GetTransform().SetPosition(glm::vec3(-6.0f, 1.5f, -3.0f));

	SceneNode* PBRGrassMeadow = new SceneNode(GrassMeadow);
	root->AddChild(PBRGrassMeadow);
	PBRGrassMeadow->GetTransform().SetPosition(glm::vec3(-8.0f, 1.5f, -3.0f));
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

 	root->Update(dt);

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

void Level::Render()
{
	lightsManager->DrawLightDepthMaps(root);
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
	BuildNodeLists(root);
	SortNodeLists();
	DrawNodes();
	ClearNodeLists();

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
			transparentNodeList.push_back(from);
		}
		else
		{
			nodeList.push_back(from);
		}
	}

	for (std::vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); ++i)
	{
		BuildNodeLists(*i);
	}
}

void Level::SortNodeLists()
{
	std::sort(transparentNodeList.rbegin(), transparentNodeList.rend(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void Level::ClearNodeLists()
{
	transparentNodeList.clear();
	nodeList.clear();
}

void Level::DrawNodes()
{
	for (const auto& i : nodeList)
	{
		DrawNode(i);
	}

	for (const auto& i : transparentNodeList)
	{
		DrawNode(i);
	}
}

void Level::DrawNode(SceneNode* n)
{
	if (n->GetMesh() || n->GetModel())
	{
		n->Draw();
	}
}

void Level::BuildRenderObjectLists()
{
	for (auto& gameObject : gameObjects)
	{
		RenderObject* rObj = gameObject->GetRenderObject();
		if (rObj == nullptr) continue;

		if (frameFrustum.InsideFrustum(rObj))
		{
			glm::vec3 dir = gameObject->GetTransform().GetPosition() - camera->GetCameraTransform().GetPosition();
			rObj->SetCameraDistance(glm::dot(dir, dir));

			objectList.push_back(rObj);
		}
	}
}

void Level::SortRenderObjectLists()
{
	std::sort(transparentObjectList.rbegin(), transparentObjectList.rend(), RenderObject::CompareByCameraDistance);
	std::sort(objectList.begin(), objectList.end(), RenderObject::CompareByCameraDistance);
}

void Level::ClearRenderObjectLists()
{
	transparentObjectList.clear();
	objectList.clear();
}

void Level::CheckSelectSkybox()
{
	if (selectSkybox == currentSkybox) return;
	
	currentSkybox = selectSkybox;

	CubeMaps[currentSkybox]->BindIrradianceMap(7);
	CubeMaps[currentSkybox]->BindprefilterMap(8);
	CubeMaps[currentSkybox]->BindBRDFLUT(9);
}
