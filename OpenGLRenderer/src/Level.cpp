#include "Level.h"
#include "SceneNode.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Mesh.h"
#include "Texture.h"
#include "glRenderer.h"
#include "Light.h"
#include "Shader.h"
#include "Model.h"

#include <algorithm>

Level::Level(glRenderer* r)
{
	renderer = r;

	camera = new Camera(0.0f, 0.0f, glm::vec3(0.0f, 2.0f, 3.0f));

	lightsManager = new LightsManager();

	skybox = new CubeMap(
		"../Resources/CubeMap/evening_right.jpg",
		"../Resources/CubeMap/evening_left.jpg",
		"../Resources/CubeMap/evening_top.jpg",
		"../Resources/CubeMap/evening_bottom.jpg",
		"../Resources/CubeMap/evening_front.jpg",
		"../Resources/CubeMap/evening_back.jpg"
	);

	skybox1 = new CubeMap("../Resources/HDR/Mondello/spiaggia_di_mondello_2k.hdr");

	floor = Mesh::GenerateFloor();
	floor->shaderIndex = 4;

	cube = Mesh::GenerateCube();
	cube->shaderIndex = 1;
	cube->AddTexture(Texture("../Resources/Textures/container2.png"));
	cube->AddTexture(Texture("../Resources/Textures/container2_specular.png", TEXTYPE_SPECULAR));

	RustedIron = Mesh::GenerateCube();
	RustedIron->shaderIndex = 6;
	RustedIron->AddTexture(Texture("../Resources/Textures/RustedIron/rustediron2_basecolor.png", TEXTYPE_ALBEDO));
	RustedIron->AddTexture(Texture("../Resources/Textures/RustedIron/rustediron2_metallic.png", TEXTYPE_METALLIC));
	RustedIron->AddTexture(Texture("../Resources/Textures/RustedIron/rustediron2_normal.png", TEXTYPE_NORMAL));
	RustedIron->AddTexture(Texture("../Resources/Textures/RustedIron/rustediron2_roughness.png", TEXTYPE_ROUGHNESS));
	RustedIron->AddTexture(Texture("../Resources/Textures/RustedIron/rustediron2_ao.png", TEXTYPE_AO));

	BrickWall = Mesh::GenerateCube();
	BrickWall->shaderIndex = 6;
	BrickWall->AddTexture(Texture("../Resources/Textures/BrickWall/brick-wall_albedo.png", TEXTYPE_ALBEDO));
	BrickWall->AddTexture(Texture("../Resources/Textures/BrickWall/brick-wall_metallic.png", TEXTYPE_METALLIC));
	BrickWall->AddTexture(Texture("../Resources/Textures/BrickWall/brick-wall_roughness.png", TEXTYPE_ROUGHNESS));
	BrickWall->AddTexture(Texture("../Resources/Textures/BrickWall/brick-wall_normal-dx.png", TEXTYPE_NORMAL));
	BrickWall->AddTexture(Texture("../Resources/Textures/BrickWall/brick-wall_height.png", TEXTYPE_HEIGHT));
	BrickWall->AddTexture(Texture("../Resources/Textures/BrickWall/brick-wall_ao.png", TEXTYPE_AO));
	
	GrassMeadow = Mesh::GenerateCube();
	GrassMeadow->shaderIndex = 6;
	GrassMeadow->AddTexture(Texture("../Resources/Textures/GrassMeadow/grass-meadow_albedo.png", TEXTYPE_ALBEDO));
	GrassMeadow->AddTexture(Texture("../Resources/Textures/GrassMeadow/grass-meadow_metallic.png", TEXTYPE_METALLIC));
	GrassMeadow->AddTexture(Texture("../Resources/Textures/GrassMeadow/grass-meadow_roughness.png", TEXTYPE_ROUGHNESS));
	GrassMeadow->AddTexture(Texture("../Resources/Textures/GrassMeadow/grass-meadow_normal-dx.png", TEXTYPE_NORMAL));
	GrassMeadow->AddTexture(Texture("../Resources/Textures/GrassMeadow/grass-meadow_height.png", TEXTYPE_HEIGHT));
	GrassMeadow->AddTexture(Texture("../Resources/Textures/GrassMeadow/grass-meadow_ao.png", TEXTYPE_AO));

	StainlessSteel = Mesh::GenerateCube();
	StainlessSteel->shaderIndex = 6;
	StainlessSteel->AddTexture(Texture("../Resources/Textures/StainlessSteel/used-stainless-steel2_albedo.png", TEXTYPE_ALBEDO));
	StainlessSteel->AddTexture(Texture("../Resources/Textures/StainlessSteel/used-stainless-steel2_metallic.png", TEXTYPE_METALLIC));
	StainlessSteel->AddTexture(Texture("../Resources/Textures/StainlessSteel/used-stainless-steel2_roughness.png", TEXTYPE_ROUGHNESS));
	StainlessSteel->AddTexture(Texture("../Resources/Textures/StainlessSteel/used-stainless-steel2_normal-dx.png", TEXTYPE_NORMAL));
	StainlessSteel->AddTexture(Texture("../Resources/Textures/StainlessSteel/used-stainless-steel2_height.png", TEXTYPE_HEIGHT));
	StainlessSteel->AddTexture(Texture("../Resources/Textures/StainlessSteel/used-stainless-steel2_ao.png", TEXTYPE_AO));

	WhiteMarble = Mesh::GenerateCube();
	WhiteMarble->shaderIndex = 6;
	WhiteMarble->AddTexture(Texture("../Resources/Textures/WhiteMarble/white-marble_albedo.png", TEXTYPE_ALBEDO));
	WhiteMarble->AddTexture(Texture("../Resources/Textures/WhiteMarble/white-marble_metallic.png", TEXTYPE_METALLIC));
	WhiteMarble->AddTexture(Texture("../Resources/Textures/WhiteMarble/white-marble_roughness.png", TEXTYPE_ROUGHNESS));
	WhiteMarble->AddTexture(Texture("../Resources/Textures/WhiteMarble/white-marble_normal-dx.png", TEXTYPE_NORMAL));
	WhiteMarble->AddTexture(Texture("../Resources/Textures/WhiteMarble/white-marble_height.png", TEXTYPE_HEIGHT));
	WhiteMarble->AddTexture(Texture("../Resources/Textures/WhiteMarble/white-marble_ao.png", TEXTYPE_AO));

	Lava = Mesh::GenerateCube();
	Lava->shaderIndex = 6;
	Lava->AddTexture(Texture("../Resources/Textures/ColumnedLavaRock/columned-lava-rock_albedo.png", TEXTYPE_ALBEDO));
	Lava->AddTexture(Texture("../Resources/Textures/ColumnedLavaRock/columned-lava-rock_metallic.png", TEXTYPE_METALLIC));
	Lava->AddTexture(Texture("../Resources/Textures/ColumnedLavaRock/columned-lava-rock_roughness.png", TEXTYPE_ROUGHNESS));
	Lava->AddTexture(Texture("../Resources/Textures/ColumnedLavaRock/columned-lava-rock_normal-dx.png", TEXTYPE_NORMAL));
	Lava->AddTexture(Texture("../Resources/Textures/ColumnedLavaRock/columned-lava-rock_height.png", TEXTYPE_HEIGHT));
	Lava->AddTexture(Texture("../Resources/Textures/ColumnedLavaRock/columned-lava-rock_ao.png", TEXTYPE_AO));

	backpack = new Model("../Resources/Models/backpack/backpack.obj");
	backpack->SetShaderIndex(1);

	root = new SceneNode();

	ConstructScene();

	LevelBeginPlay();
}

Level::~Level()
{
	delete camera;
	delete lightsManager;
	delete skybox;
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
	lightsManager->AddDirectionalLight(glm::vec3(-80.0f, 0.0f, 0.0f), glm::vec3(3.0f, 3.0f, 3.0f));
	lightsManager->AddSpotLight(glm::vec3(3.0f, 2.0f, 0.0f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f), 30.0f, 35.0f, 0.07f, 0.017f);
	lightsManager->AddPointLight(glm::vec3(-3.0f, 2.0f, 1.0f), glm::vec3(3.0f, 0.0f, 1.5f), 0.09f, 0.032f);


	SceneNode* Floor = new SceneNode(floor);
	root->AddChild(Floor);
	Floor->GetTransform().SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));

	SceneNode* container = new SceneNode(cube);
	root->AddChild(container);
	container->GetTransform().SetPosition(glm::vec3( 4.0f, 1.5f, -3.0f));

	//SceneNode* backpackModel = new SceneNode(backpack);
	//root->AddChild(backpackModel);
	//backpackModel->GetTransform().SetPosition(glm::vec3(6.0f, 1.5f, -3.0f));

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

	skybox1->BindIrradianceMap(6); 
	skybox1->BindprefilterMap(7);
	skybox1->BindBRDFLUT(8);
}

void Level::Render()
{
	lightsManager->DrawLightDepthMaps(root);
	lightsManager->Update();
	renderer->SetSceneBufferReady();
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
	skybox1->Draw();
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
