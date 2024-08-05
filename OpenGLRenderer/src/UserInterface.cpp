#include "UserInterface.h"
#include "window.h"
#include "Level.h"
#include "glRenderer.h"
#include "Light.h"

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
	ImGui::SeparatorText("");

	if (ImGui::CollapsingHeader("Global Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Enable gamma correction", &(renderer->bGammaCorrection));
		ImGui::SliderFloat("Exposure amount", &(renderer->exposure), 0.0f, 10.0f);
	}
	
	ImGui::Spacing();
	ImGui::SeparatorText("");

	if (ImGui::CollapsingHeader("Level Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		SetSkyboxMenu();
		SetLightMenu();
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
	if (ImGui::TreeNode("Lights"))
	{
		if (ImGui::TreeNode("Directional Lights"))
		{
			std::vector<DirLight*>& dirLights = lightManagers->GetDirlights();
			for (size_t i = 0; i < dirLights.size(); ++i)
			{
				std::string name = "Directional light " + std::to_string(i);
				ImGui::SeparatorText(name.data());

				glm::vec3 pos = dirLights[i]->GetLightPosition();
				float lPos[3] = { pos.x, pos.y, pos.z };
				ImGui::InputFloat3("Position", lPos);
				dirLights[i]->SetLightPosition(glm::vec3(lPos[0], lPos[1], lPos[2]));

				glm::vec3 rot = dirLights[i]->GetLightRotation();
				float lRot[3] = { rot.x, rot.y, rot.z };
				ImGui::InputFloat3("Rotation", lRot);
				dirLights[i]->SetLightRotation(glm::vec3(lRot[0], lRot[1], lRot[2]));

				glm::vec3 col = dirLights[i]->GetLightColor();
				float lCol[3] = { col.x, col.y, col.z };
				ImGui::ColorEdit3("Color", lCol, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
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
				ImGui::InputFloat3("Position", lPos);
				spotLights[i]->SetLightPosition(glm::vec3(lPos[0], lPos[1], lPos[2]));

				glm::vec3 rot = spotLights[i]->GetLightRotation();
				float lRot[3] = { rot.x, rot.y, rot.z };
				ImGui::InputFloat3("Rotation", lRot);
				spotLights[i]->SetLightRotation(glm::vec3(lRot[0], lRot[1], lRot[2]));

				glm::vec3 col = spotLights[i]->GetLightColor();
				float lCol[3] = { col.x, col.y, col.z };
				ImGui::ColorEdit3("Color", lCol, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
				spotLights[i]->SetLightColor(glm::vec3(lCol[0], lCol[1], lCol[2]));

				if (ImGui::TreeNode("Light properties"))
				{
					//ImGui::AlignTextToFramePadding();
					ImGui::Text("Inner cutoff"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat("", &spotLights[i]->innerCutOffInDegree); ImGui::SameLine();
					ImGui::Text("Outer cutoff"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat("", &spotLights[i]->outerCutOffInDegree);
					spotLights[i]->innerCutOff = glm::cos(glm::radians(spotLights[i]->innerCutOffInDegree));
					spotLights[i]->outerCutOff = glm::cos(glm::radians(spotLights[i]->outerCutOffInDegree));

					ImGui::Text("Attenuation: linear"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat("", &spotLights[i]->linear); ImGui::SameLine();
					ImGui::Text("quadratic"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat("", &spotLights[i]->quadratic);

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
				ImGui::InputFloat3("Position", lPos);
				pointLights[i]->SetLightPosition(glm::vec3(lPos[0], lPos[1], lPos[2]));

				glm::vec3 col = pointLights[i]->GetLightColor();
				float lCol[3] = { col.x, col.y, col.z };
				ImGui::ColorEdit3("Color", lCol, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR);
				pointLights[i]->SetLightColor(glm::vec3(lCol[0], lCol[1], lCol[2]));

				if (ImGui::TreeNode("Light properties"))
				{
					ImGui::Text("Attenuation: linear"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat("", &pointLights[i]->linear); ImGui::SameLine();
					ImGui::Text("quadratic"); ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat("", &pointLights[i]->quadratic);
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
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