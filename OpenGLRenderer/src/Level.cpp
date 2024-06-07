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

	floor = Mesh::GenerateFloor();
	floor->shaderIndex = 4;

	cube = Mesh::GenerateCube();
	cube->shaderIndex = 0;

	tex = new Texture("../Resources/Textures/brick.tga");

	backpack = new Model("../Resources/Models/backpack/backpack.obj");

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
	delete tex;
	delete root;
}

void Level::ConstructScene()
{
	lightsManager->AddDirectionalLight(glm::vec3(-70.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f));
	lightsManager->AddSpotLight(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.0f), 30.0f, 35.0f, 0.35f, 0.44f);
	lightsManager->AddPointLight(glm::vec3(1.0f, 3.0f, 1.0f), glm::vec3(0.0f, 0.8f, 0.5f), 0.35f, 0.44f);


	SceneNode* Floor = new SceneNode(floor);
	root->AddChild(Floor);
	Floor->GetTransform().SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));

	SceneNode* container = new SceneNode(cube);
	root->AddChild(container);
	container->GetTransform().SetPosition(glm::vec3(0.0f, 3.0f, -3.0f));
}

void Level::Update(float dt)
{
	camera->UpdateCamera(dt);
	camera->UploadViewMatrix(renderer->GetUboMatrices());
	lightsManager->Update();

 	root->Update(dt);

	Render();
}

void Level::LevelBeginPlay()
{
	lightsManager->Init();

	camera->UploadProjectionMatrix(renderer->GetUboMatrices());
}

void Level::Render()
{
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
	skybox->Draw();
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
