#include "StateOption.h"

#pragma warning(push)
#pragma warning(disable:26495)
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#pragma warning(pop)
#include "App.h"
#include "GLFW/glfw3.h"
#include "ResourceGFX.h"//clearWindow
#include <string>

//Init Static
bool StateOption::m_popWindow = false;

void StateOption::OnBegin()
{
	glfwSetInputMode(App::GetInstance()->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_color = { 0.f,0.f,0.f,1.f };
}

GameState StateOption::OnUpdate(float /*deltaTime*/)
{
	ClearWindow(m_color);

	GameState nextState = GameState::SETTINGS;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2((float)App::GetInstance()->m_windowWidth, (float)App::GetInstance()->m_windowHeight));
	bool settings = true;
	ImGui::Begin("Settings", &settings, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	if (App::GetInstance()->GetInputManager()->m_ChangeKey)
	{
		ImGui::Text("Press one Key:");
	}
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 17));
	ImGui::Text("Move Player Foward:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 12));

	m_keyName = VerifyKey(InputManager::KeyAction::MOVE_PLAYER_FORWARD);
	if (ImGui::Button(m_keyName, ImVec2(100, 20)))
	{
		App::GetInstance()->GetInputManager()->m_ChangeKey = true;
		App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::MOVE_PLAYER_FORWARD);
	}

	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 7.045f));
	ImGui::Text("Move Player Backward:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 6));

	m_keyName = VerifyKey(InputManager::KeyAction::MOVE_PLAYER_BACKWARD);
	if (ImGui::Button(m_keyName, ImVec2(100,20)))
	{
		App::GetInstance()->GetInputManager()->m_ChangeKey = true;
		App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::MOVE_PLAYER_BACKWARD);
	}
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 4.44f));
	ImGui::Text("Move Player Right:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 4));


	m_keyName = VerifyKey(InputManager::KeyAction::MOVE_PLAYER_RIGHT);
	if (ImGui::Button(m_keyName, ImVec2(100, 20)))
	{
		App::GetInstance()->GetInputManager()->m_ChangeKey = true;
		App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::MOVE_PLAYER_RIGHT);
	}
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 3.24f));
	ImGui::Text("Move Player Left:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 3));


	m_keyName = VerifyKey(InputManager::KeyAction::MOVE_PLAYER_LEFT);
	if (ImGui::Button(m_keyName, ImVec2(100, 20)))
	{
		App::GetInstance()->GetInputManager()->m_ChangeKey = true;
		App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::MOVE_PLAYER_LEFT);
	}
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 2.552f));
	ImGui::Text("Player Jump:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 2.4f));


	m_keyName = VerifyKey(InputManager::KeyAction::PLAYER_JUMP);
	if (ImGui::Button(m_keyName, ImVec2(100, 20)))
	{
		App::GetInstance()->GetInputManager()->m_ChangeKey = true;
		App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::PLAYER_JUMP);
	}
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 2.12f));
	ImGui::Text("Move Camera Foward:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 2));


	m_keyName = VerifyKey(InputManager::KeyAction::MOVE_CAMERA_FORWARD);
	if (ImGui::Button(m_keyName, ImVec2(100, 20)))
	{
		App::GetInstance()->GetInputManager()->m_ChangeKey = true;
		App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::MOVE_CAMERA_FORWARD);
	}
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 1.7905f));
	ImGui::Text("Move Camera Backward:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 1.714f));


	m_keyName = VerifyKey(InputManager::KeyAction::MOVE_CAMERA_BACKWARD);
	if (ImGui::Button(m_keyName, ImVec2(100, 20)))
	{
		App::GetInstance()->GetInputManager()->m_ChangeKey = true;
		App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::MOVE_CAMERA_BACKWARD);
	}
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 1.558f));
	ImGui::Text("Move Camera Right:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 1.5f));

	m_keyName = VerifyKey(InputManager::KeyAction::MOVE_CAMERA_RIGHT);
	if (ImGui::Button(m_keyName, ImVec2(100, 20)))
	{
		App::GetInstance()->GetInputManager()->m_ChangeKey = true;
		App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::MOVE_CAMERA_RIGHT);
	}
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 1.379f));
	ImGui::Text("Move Camera Left:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 1.333f));

	m_keyName = VerifyKey(InputManager::KeyAction::MOVE_CAMERA_LEFT);
	if (ImGui::Button(m_keyName, ImVec2(100, 20)))
	{
		App::GetInstance()->GetInputManager()->m_ChangeKey = true;
		App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::MOVE_CAMERA_LEFT);
	}
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 - 200, (float)App::GetInstance()->m_windowHeight / 1.235f));
	ImGui::Text("Mode Editor:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 - 200, (float)App::GetInstance()->m_windowHeight / 1.203f));

	m_keyName = VerifyKey(InputManager::KeyAction::MODE_EDIT);
	if (ImGui::Button(m_keyName, ImVec2(100, 20)))
	{
		App::GetInstance()->GetInputManager()->m_ChangeKey = true;
		App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::MODE_EDIT);
	}

	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 2 -200, (float)App::GetInstance()->m_windowHeight / 1.12f ));
	if (ImGui::Button("Return Menu", ImVec2(100, 20)))
	{
		nextState = GameState::MENU;
	}
	ImGui::End();

	if (m_popWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2((float)App::GetInstance()->m_windowWidth, (float)App::GetInstance()->m_windowHeight));
		ImGui::Begin("WindowPOP", &m_popWindow, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth /2 - 25, (float)App::GetInstance()->m_windowHeight /2));
		ImGui::Text("Input already assigned:");
		ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth /2, (float)App::GetInstance()->m_windowHeight/1.846f));
		if (ImGui::Button("OK", ImVec2(100, 20)))
		{
			m_popWindow = false;
		}
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return nextState;
}

void StateOption::OnEnd()
{
}


void StateOption::PopWindow()
{
	m_popWindow = true;
}

const char* StateOption::VerifyKey(InputManager::KeyAction action)
{
	m_keycode = App::GetInstance()->GetInputManager()->GetMap().find(action)->second;
	switch (m_keycode)
	{
	case 32:
		return "Space";
		break;
	case 262:
		return "Arrow_Right";
		break;
	case 263:
		return "Arrow_Left";
		break;
	case 264: 
		return "Arrow_Down";
		break;
	case 265:
		return "Arrow_UP";
		break;
	case 290:
		return "F1";
		break;
	case 291:
		return "F2";
		break;
	case 292:
		return "F3";
		break;
	case 293:
		return "F4";
		break;
	case 294:
		return "F5";
		break;
	case 295:
		return "F6";
		break;
	case 296:
		return "F7";
		break;
	case 297:
		return "F8";
		break;
	case 298:
		return "F9";
		break;
	case 299:
		return "F10";
		break;
	case 300:
		return "F11";
		break;
	case 301:
		return "F12";
		break;
	default:
		if (glfwGetKeyName(m_keycode, 0) == NULL)
		{
			App::GetInstance()->GetInputManager()->GetMap()[action] = GLFW_KEY_UNKNOWN;
			/* 
			const char* result = "null";
			for (size_t i = 0; i < (int)action; i++)
			{
				result += ' ';
			}*/
			return "null";
		}
		return glfwGetKeyName(m_keycode, 0);
		break;
	}
}
