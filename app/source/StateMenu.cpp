#include "StateMenu.h"
#include "ResourceGFX.h"
#pragma warning(push)
#pragma warning(disable:26495)
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#pragma warning(pop)
#include "App.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "glad/glad.h"
#include "Resources/TextureManager.h"

StateMenu::StateMenu(GLFWwindow* window)
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return;
	}

	const char* glsl_version = "#version 450";
					/*INIT ImGUI*/
	//----------------------------------------------------------------------------------------------
	//Setup  Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	//Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//Setup Platform/Renderer bindigs
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	//----------------------------------------------------------------------------------------------
}

void StateMenu::OnBegin()
{
								//--- Render Cursor ---//
	glfwSetInputMode(App::GetInstance()->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


GameState StateMenu::OnUpdate(float /*deltaTime*/)
{
	GameState nextState = GameState::MENU;

	//--- GlClear ---//
	ClearWindow(m_color);


	//--- Set new  for ImGUI ---//
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool menu = true;

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2((float)App::GetInstance()->m_windowWidth, (float)App::GetInstance()->m_windowHeight));
	ImGui::Begin("MENU", &menu, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2, (float)App::GetInstance()->m_windowHeight / 2.4f));


														//---- Button NEW Game ----//
	if (ImGui::Button("New Game!", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		nextState = GameState::PLAY;
		App::GetInstance()->m_gameInit = false;
	}

														//---- Button LOAD GAME ----//
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2, (float)App::GetInstance()->m_windowHeight / 2));
	if (ImGui::Button("Load Game!", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		nextState = GameState::PLAY;
		App::GetInstance()->m_save.ImportSave(App::GetInstance()->m_scene.GetRoot(), &(App::GetInstance()->m_scene.m_camera));
	}

														//---- Button Options ----//
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2, (float)App::GetInstance()->m_windowHeight / 1.71f));
	if (ImGui::Button("Options", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		nextState = GameState::SETTINGS;
	}

														//---- Button QUIT ----//
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2, (float)App::GetInstance()->m_windowHeight / 1.5f));
	if (ImGui::Button("Quit", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		glfwSetWindowShouldClose(App::GetInstance()->m_window, true);
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return nextState;
}

void StateMenu::OnEnd()
{
}
	