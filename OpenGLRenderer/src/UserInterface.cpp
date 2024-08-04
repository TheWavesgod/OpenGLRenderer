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

	if (ImGui::CollapsingHeader("Global Settings", 1))
	{
		ImGui::Checkbox("Enable gamma correction", &(renderer->bGammaCorrection));
		ImGui::SliderFloat("Exposure amount", &(renderer->exposure), 0.0f, 10.0f);
	}
	
	if (ImGui::CollapsingHeader("Level Settings", 1))
	{
		SetSkyboxMenu();
		SetLightMenu();
	}
	
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
			for (auto dirLight : dirLights)
			{

			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Spot Lights"))
		{
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