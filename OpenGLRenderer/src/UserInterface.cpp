#include "UserInterface.h"
#include "window.h"
#include "Level.h"
#include "glRenderer.h"
#include "Light.h"
#include "Material.h"
#include "Model.h"

UserInterface* UserInterface::uiPtr = nullptr;

UserInterface* UserInterface::CreateUserInterface(window* w, glRenderer* renderer, Level& l)
{
	static UserInterface ui(w, renderer, l);
	uiPtr = &ui;
	return &ui;
}

void UserInterface::StartNewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UserInterface::Render()
{
	SetMenu();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::Shutdown()
{	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UserInterface::SetMenu()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::Begin("OpenGL Render Engine");

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::Spacing();
	ImGui::Text("Press TAB to switch mouse cursor state");
	ImGui::Spacing();
	ImGui::Text("Hold Mouse Right Buttom to activate free camera view");
	ImGui::Spacing();
	ImGui::Text("W A S D move the camera, Q E raise or lower the camera");
	ImGui::Spacing();
	ImGui::SeparatorText("");

	if (ImGui::CollapsingHeader("Global Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Enable gamma correction", &(renderer->bGammaCorrection));
		ImGui::SliderFloat("Exposure amount", &(renderer->exposure), 0.0f, 10.0f);
	}

	ImGui::Spacing();
	ImGui::SeparatorText("");

	if (ImGui::CollapsingHeader("Assets", ImGuiTreeNodeFlags_DefaultOpen))
	{
		SetMaterialMenu();
		SetModelMenu();
	}
	
	ImGui::Spacing();
	ImGui::SeparatorText("");

	if (ImGui::CollapsingHeader("Level Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		SetSkyboxMenu();
		SetLightMenu();
		SetGameObjectsMenu();
	}

	ImGui::Spacing();
	ImGui::SeparatorText("");
	if (ImGui::Button("Close", ImVec2(80.0f, 26.0f)))
	{
		w->SetWindowShouldClose(true);
	}
	ImGui::End();
}

const char* skyboxes[] = { "RockHill", "LivingRoom", "DarkPlace", "Gallery" };

void UserInterface::SetSkyboxMenu()
{
	if (ImGui::TreeNode("Skybox"))
	{
		ImGui::Combo("Select Skybox", &level.selectSkybox, skyboxes, IM_ARRAYSIZE(skyboxes));

		ImGui::TreePop();
	}
}

void UserInterface::SetLightMenu()
{
	LightsManager* lightManagers = level.GetLightsManager();
	if (ImGui::TreeNodeEx("Lights", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::TreeNode("Directional Lights"))
		{
			std::vector<DirLight*>& dirLights = lightManagers->GetDirlights();
			for (size_t i = 0; i < dirLights.size(); ++i)
			{
				std::string name = "Directional light " + std::to_string(i);
				ImGui::SeparatorText(name.data());

				glm::vec3 rot = dirLights[i]->GetLightRotation();
				float lRot[3] = { rot.x, rot.y, rot.z };
				ImGui::InputFloat3(("Rotation ##dirLight" + std::to_string(i)).c_str(), lRot);
				dirLights[i]->SetLightRotation(glm::vec3(lRot[0], lRot[1], lRot[2]));

				glm::vec3 col = dirLights[i]->GetLightColor();
				float lCol[3] = { col.x, col.y, col.z };
				ImGui::ColorEdit3(("Color ##dirLight" + std::to_string(i)).c_str(), lCol, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
				dirLights[i]->SetLightColor(glm::vec3(lCol[0], lCol[1], lCol[2]));
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Spot Lights"))
		{
			std::vector<SpotLight*>& spotLights = lightManagers->GetSpotlights();
			for (size_t i = 0; i < spotLights.size(); ++i)
			{
				std::string name = "Spot light " + std::to_string(i);
				ImGui::SeparatorText(name.data());

				glm::vec3 pos = spotLights[i]->GetLightPosition();
				float lPos[3] = { pos.x, pos.y, pos.z };
				ImGui::InputFloat3(("Position ##spotLight" + std::to_string(i)).c_str(), lPos);
				spotLights[i]->SetLightPosition(glm::vec3(lPos[0], lPos[1], lPos[2]));

				glm::vec3 rot = spotLights[i]->GetLightRotation();
				float lRot[3] = { rot.x, rot.y, rot.z };
				ImGui::InputFloat3(("Rotation ##spotLight" + std::to_string(i)).c_str(), lRot);
				spotLights[i]->SetLightRotation(glm::vec3(lRot[0], lRot[1], lRot[2]));

				glm::vec3 col = spotLights[i]->GetLightColor();
				float lCol[3] = { col.x, col.y, col.z };
				ImGui::ColorEdit3(("Color ##spotLight" + std::to_string(i)).c_str(), lCol, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
				spotLights[i]->SetLightColor(glm::vec3(lCol[0], lCol[1], lCol[2]));

				if (ImGui::TreeNode(("Light properties##spotLight" + std::to_string(i)).c_str()))
				{
					//ImGui::AlignTextToFramePadding();
					ImGui::Text("Inner cutoff"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat(("IC##spotLight" + std::to_string(i)).c_str(), &spotLights[i]->innerCutOffInDegree); ImGui::SameLine();
					ImGui::Text("Outer cutoff"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat(("OC##spotLight" + std::to_string(i)).c_str(), &spotLights[i]->outerCutOffInDegree);
					spotLights[i]->innerCutOff = glm::cos(glm::radians(spotLights[i]->innerCutOffInDegree));
					spotLights[i]->outerCutOff = glm::cos(glm::radians(spotLights[i]->outerCutOffInDegree));

					ImGui::Text("Attenuation: linear"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat(("AL##spotLight" + std::to_string(i)).c_str(), &spotLights[i]->linear); ImGui::SameLine();
					ImGui::Text("quadratic"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat(("AQ##spotLight" + std::to_string(i)).c_str(), &spotLights[i]->quadratic);

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Point Lights"))
		{
			std::vector<PointLight*>& pointLights = lightManagers->GetPointlights();
			for (size_t i = 0; i < pointLights.size(); ++i)
			{
				std::string name = "Point light " + std::to_string(i);
				ImGui::SeparatorText(name.data());

				glm::vec3 pos = pointLights[i]->GetLightPosition();
				float lPos[3] = { pos.x, pos.y, pos.z };
				ImGui::InputFloat3(("Position ##pointLight" + std::to_string(i)).c_str(), lPos);
				pointLights[i]->SetLightPosition(glm::vec3(lPos[0], lPos[1], lPos[2]));

				glm::vec3 col = pointLights[i]->GetLightColor();
				float lCol[3] = { col.x, col.y, col.z };
				ImGui::ColorEdit3(("Color ##pointLight" + std::to_string(i)).c_str(), lCol, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
				pointLights[i]->SetLightColor(glm::vec3(lCol[0], lCol[1], lCol[2]));

				if (ImGui::TreeNode(("Light properties##pointLight" + std::to_string(i)).c_str()))
				{
					ImGui::Text("Attenuation: linear"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat(("AL##pointLight" + std::to_string(i)).c_str(), &pointLights[i]->linear); ImGui::SameLine();
					ImGui::Text("quadratic"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat(("AQ##pointLight" + std::to_string(i)).c_str(), &pointLights[i]->quadratic);
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void UserInterface::SetMaterialMenu()
{
	std::vector<Material*> materials = level.GetMaterials();
	if (ImGui::TreeNode("Materials"))
	{
		for (size_t i = 0; i < materials.size(); ++i)
		{
			std::string name = "Material " + std::to_string(i) + " : " + materials[i]->GetName();
			ImGui::SeparatorText(name.data());

			ImGui::Text("PBR Render: "); ImGui::SameLine();
			ImGui::BeginDisabled(true);
			bool bUsePBR = materials[i]->GetUsePBR();
			ImGui::Checkbox(("##bUsePBR" + std::to_string(i)).c_str(), &bUsePBR);
			ImGui::EndDisabled(); 

			if (!materials[i]->useAlbedo)
			{
				ImGui::Text("Base Color: "); ImGui::SameLine();
				glm::vec3 col = materials[i]->baseColor;
				float lCol[3] = { col.x, col.y, col.z };
				if (ImGui::ColorEdit3(("##BaseColor" + std::to_string(i)).c_str(), 
					lCol, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR))
				{
					materials[i]->baseColor = glm::vec3(lCol[0], lCol[1], lCol[2]);
				}
			}

			if (!materials[i]->useMetallic)
			{
				ImGui::Text("Metallic: "); ImGui::SameLine();
				float met = materials[i]->matellic;
				if (ImGui::SliderFloat(("##metallic" + std::to_string(i)).c_str(), &met, 0.0f, 1.0f))
				{
					materials[i]->matellic = met;
				}
			}

			if (!materials[i]->useRoughness)
			{
				ImGui::Text("Roughness: "); ImGui::SameLine();
				float rough = materials[i]->roughness;
				if (ImGui::SliderFloat(("##Roughness" + std::to_string(i)).c_str(), &rough, 0.0f, 1.0f))
				{
					materials[i]->roughness = rough;
				}
			}

			if (materials[i]->useHeight)
			{
				ImGui::Text("Height mapping scale: "); ImGui::SameLine();
				float heightScale = materials[i]->GetHeightScale();
				if (ImGui::SliderFloat(("##heightScale" + std::to_string(i)).c_str(), &heightScale, 0.0f, 1.0f))
				{
					materials[i]->SetHeightScale(heightScale);
				}
			}

			if (materials[i]->useEmissive)
			{
				ImGui::Text("Emissive intensity scale: "); ImGui::SameLine();
				float emissiveScale = materials[i]->GetEmissiveScale();
				if (ImGui::SliderFloat(("##emissiveScale" + std::to_string(i)).c_str(), &emissiveScale, 0.0f, 10.0f))
				{
					materials[i]->SetEmissiveScale(emissiveScale);
				}
			}

			if (materials[i]->GetTransparent())
			{
				ImGui::Text("Alpha: "); ImGui::SameLine();
				float a = materials[i]->alpha;
				if (ImGui::SliderFloat(("##alpha" + std::to_string(i)).c_str(), &a, 0.0f, 1.0f))
				{
					materials[i]->alpha = a;
				}
			}

			if (ImGui::TreeNode(("Texture File Path ##file" + std::to_string(i)).c_str()))
			{
				const std::string* files = materials[i]->GetFilePathes();
				ImGui::BeginDisabled(true);

				if (bUsePBR)
				{
					for (int j = TEXTYPE_ALBEDO; j < TEXTYPE_MAX; ++j)
					{
						std::string name = Texture::GetTypeString((TextureType)j);
						ImGui::Text((name + ": ").c_str()); ImGui::SameLine();
						ImGui::Text(files[j].c_str());
					}
				}

				ImGui::EndDisabled();
				ImGui::TreePop();
			}

			ImGui::SeparatorText("");
			ImGui::Spacing();
		}
		ImGui::TreePop();
	}
}

void UserInterface::SetModelMenu()
{
	if (ImGui::TreeNodeEx("Models"))
	{
		std::vector<Model*>& models = level.GetModels();
		std::vector<Material*>& materials = level.GetMaterials();
		std::vector<const char*> materialsName;
		materialsName.resize(materials.size());
		for (size_t i = 0; i < materials.size(); ++i)
		{
			materialsName[i] = materials[i]->GetName().c_str();
		}

		for (size_t i = 0; i < models.size(); ++i)
		{
			std::string name = "Model " + std::to_string(i) + " : " + models[i]->name;
			ImGui::SeparatorText(name.data());

			if (ImGui::TreeNode(("Materials Slot##MaterialSlot" + std::to_string(i)).c_str()))
			{
				std::vector<int>& material = *(models[i]->GetMaterialIndices());
				for (size_t j = 0; j < material.size(); ++j)
				{
					ImGui::Text(("Slot " + std::to_string(j)).c_str()); ImGui::SameLine();
					ImGui::Combo(("##Model" + std::to_string(i) + "slot" + std::to_string(j)).c_str(),
						&material[j], materialsName.data(), materialsName.size());
				}

				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

void UserInterface::SetGameObjectsMenu()
{
	if (ImGui::TreeNodeEx("Game Objects", ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::vector<GameObject*>& objs = level.GetGameObjects();
		for (size_t i = 0; i < objs.size(); ++i)
		{
			if (ImGui::TreeNode((objs[i]->GetName() + "##GameObject" + std::to_string(i)).c_str()))
			{
				

				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}

UserInterface::UserInterface(window* w, glRenderer* renderer, Level& l) : w(w), level(l), renderer(renderer)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(w->GetGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}