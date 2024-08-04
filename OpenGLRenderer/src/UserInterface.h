#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

class window;
class Level;
class glRenderer;

class UserInterface
{
public:
	static UserInterface* CreateUserInterface(window* w, glRenderer* renderer, Level& l);
	static UserInterface* GetUserInterface() { return uiPtr; }

	void StartNewFrame();
	void Render();
	void Shutdown();

	UserInterface(const UserInterface&) = delete;
	UserInterface& operator =(const UserInterface&) = delete;

protected:
	static UserInterface* uiPtr;
	
	window* w;
	Level& level;
	glRenderer* renderer;

	void SetMenu();
	void SetSkyboxMenu();
	void SetLightMenu();

private:
	UserInterface(window* w, glRenderer* renderer, Level& l);
};

