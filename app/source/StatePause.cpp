#include "StatePause.h"
#include "App.h"
#include "Player.h"
#include <iostream>
#include "Resources/TextureManager.h"

#pragma warning(push)
#pragma warning(disable:26495)
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#pragma warning(pop)

//Init Static
bool StatePause::m_popWindowSave = false;

void StatePause::OnBegin()
{
	glfwSetInputMode(App::GetInstance()->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


GameState StatePause::OnUpdate(float /*deltaTime*/)
{
	GameState nextState = GameState::PAUSE;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	bool pause = true;
	ImGui::SetNextWindowPos(ImVec2(-5, -2));
	ImGui::SetNextWindowSize({ (float)App::GetInstance()->m_windowWidth + 20, (float)App::GetInstance()->m_windowHeight + 20 });
	ImGui::SetNextWindowBgAlpha(0.02f);
	ImGui::Begin("Settings", &pause, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove );

											//----- Buttton "RESUME" ------//
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 - 50, (float)App::GetInstance()->m_windowHeight / 3.f));
	if (ImGui::Button("Reprendre partie", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		nextState = GameState::PLAY;
	}

												//----- Buttton "Sauvegarder partie" ------//
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 - 50, (float)App::GetInstance()->m_windowHeight / 2.f));
	if (ImGui::Button("Sauvegarder partie", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		App::GetInstance()->m_save.ExportSave();
		m_popWindowSave = true;
	}
											//----- Buttton "Menu" ------//
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 - 50, (float)App::GetInstance()->m_windowHeight / 2.4f));
	if (ImGui::Button("Retourner au Menu", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		App::GetInstance()->m_save.DealocateGameObjectMemory();
		App::GetInstance()->m_scene.GetRoot()->~Graph();
		App::GetInstance()->m_scene.InitScene();
		nextState = GameState::MENU;
	}
	
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2.f - 50, (float)App::GetInstance()->m_windowHeight / 1.71f));
	if (ImGui::Button("Charger partie", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		App::GetInstance()->m_save.DealocateGameObjectMemory();
		App::GetInstance()->m_scene.GetRoot()->~Graph();
		App::GetInstance()->m_scene.InitScene();
		App::GetInstance()->m_save.ImportSave(App::GetInstance()->m_scene.GetRoot(), &(App::GetInstance()->m_scene.m_camera));
		App::GetInstance()->m_gameInit = true;
		nextState = GameState::PLAY;
	}
	ImGui::End();

	if (m_popWindowSave)
	{
		PopWindow();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return nextState;
}

void StatePause::OnEnd()
{
}

void StatePause::PopWindow()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2((float)App::GetInstance()->m_windowWidth, (float)App::GetInstance()->m_windowHeight));
	ImGui::Begin("WindowPOP", &m_popWindowSave, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 - 25, (float)App::GetInstance()->m_windowHeight / 2));
	ImGui::Text("Back up scene its OK! ;)");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2, 325));
	if (ImGui::Button("OK", ImVec2(100, 20)))
	{
		m_popWindowSave = false;
	}
	ImGui::End();
}