#include "InputManager.h"
#include "LowRenderer/Camera.h"
#include "State.h"
#include "App.h"
#include "Resources/Scene.h"
#include "Player.h"
#include "StateOption.h"

//camera settings
extern bool g_active_mouse;
extern bool g_mouse_cursor;
extern bool g_firstMouse;
extern float g_lastX;
extern float g_lastY;
extern bool wireframe;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Game::Player* player = nullptr;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
InputManager::InputManager(GLFWwindow* window)
{
	m_settings[KeyAction::MOVE_CAMERA_FORWARD] = GLFW_KEY_UP;
	m_settings[KeyAction::MOVE_CAMERA_RIGHT] = GLFW_KEY_RIGHT;
	m_settings[KeyAction::MOVE_CAMERA_LEFT] = GLFW_KEY_LEFT;
	m_settings[KeyAction::MOVE_CAMERA_BACKWARD] = GLFW_KEY_DOWN;

	m_settings[KeyAction::MOVE_PLAYER_FORWARD] = GLFW_KEY_W;
	m_settings[KeyAction::MOVE_PLAYER_RIGHT] = GLFW_KEY_D;
	m_settings[KeyAction::MOVE_PLAYER_LEFT] = GLFW_KEY_A;
	m_settings[KeyAction::MOVE_PLAYER_BACKWARD] = GLFW_KEY_S;
	m_settings[KeyAction::PLAYER_JUMP] = GLFW_KEY_SPACE;
	m_settings[KeyAction::MODE_EDIT] = GLFW_KEY_E;

	//callback for the scroll
	//glfwSetScrollCallback(window, scroll_callback);

	//callback for the mouse
	//glfwSetCursorPosCallback(window, mouse_callback);

	//callback for the inputs
	glfwSetKeyCallback(window, key_callback);
} 

void InputManager::ProcessInput(GLFWwindow* window, float delta)
{	
	switch (App::GetInstance()->GetFSM()->GetState())
	{
	case GameState::PLAY:
		if (glfwGetKey(window, m_settings[KeyAction::MOVE_CAMERA_FORWARD]) == GLFW_PRESS)
		{
			App::GetInstance()->m_scene.m_camera.MoveCamera(LowRenderer::MovCamera::FORWARD, delta);
		}
		if (glfwGetKey(window, m_settings[KeyAction::MOVE_CAMERA_BACKWARD]) == GLFW_PRESS)
		{
			App::GetInstance()->m_scene.m_camera.MoveCamera(LowRenderer::MovCamera::BACKWARD, delta);
		}
		if (glfwGetKey(window, m_settings[KeyAction::MOVE_CAMERA_LEFT]) == GLFW_PRESS)
		{
			App::GetInstance()->m_scene.m_camera.MoveCamera(LowRenderer::MovCamera::LEFT, delta);
		}
		if (glfwGetKey(window, m_settings[KeyAction::MOVE_CAMERA_RIGHT]) == GLFW_PRESS)
		{
			App::GetInstance()->m_scene.m_camera.MoveCamera(LowRenderer::MovCamera::RIGHT, delta);
		}
		player = Game::Player::GetInstance();
		if (player != nullptr)
		{
			if (glfwGetKey(window, m_settings[KeyAction::MOVE_PLAYER_FORWARD]) == GLFW_PRESS)
			{
				player->Move(Game::PlayerMove::MOVE_FORWARD);
			}
			if (glfwGetKey(window, m_settings[KeyAction::MOVE_PLAYER_BACKWARD]) == GLFW_PRESS)
			{
				player->Move(Game::PlayerMove::MOVE_BACKWARD);
			}
			if (glfwGetKey(window, m_settings[KeyAction::MOVE_PLAYER_RIGHT]) == GLFW_PRESS)
			{
				player->Move(Game::PlayerMove::MOVE_RIGHT);
			}
			if (glfwGetKey(window, m_settings[KeyAction::MOVE_PLAYER_LEFT]) == GLFW_PRESS)
			{
				player->Move(Game::PlayerMove::MOVE_LEFT);
			}
			if (glfwGetKey(window, m_settings[KeyAction::PLAYER_JUMP]) == GLFW_PRESS)
			{
				player->Jump();
			}
		}
		break;
	default:
		break;
	}
}

void InputManager::SetKeyChange(KeyAction key)
{
	m_keyChange = key;
}

InputManager::KeyAction InputManager::GetKeyChange() const
{
	return m_keyChange;
}

void InputManager::ChangeInput(KeyAction action, int key)
{
	for (int i = 0; i < (int)KeyAction::NOKEY; i++)
	{
		if (m_settings[(KeyAction)i] == key)
		{
			StateOption::PopWindow();
			m_keyChange = action;
			return;
		}
	}
	m_settings[action] = key;
}

std::unordered_map<InputManager::KeyAction, int> InputManager::GetMap() const
{
	return m_settings;
}

void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	switch (App::GetInstance()->GetFSM()->GetState())
	{
	case GameState::PLAY:
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			App::GetInstance()->GetFSM()->ChangeState(GameState::PAUSE);
		}
		if (key == (App::GetInstance()->GetInputManager()->GetMap()[InputManager::KeyAction::MODE_EDIT]) && action == GLFW_PRESS)
		{
			App::GetInstance()->GetFSM()->ChangeState(GameState::EDIT);
		}
		break;
	case GameState::MENU:
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			exit(0);
		}
		break;
	case GameState::SETTINGS:
		if (App::GetInstance()->GetInputManager()->GetKeyChange() != InputManager::KeyAction::NOKEY)
		{
			App::GetInstance()->GetInputManager()->ChangeInput(App::GetInstance()->GetInputManager()->GetKeyChange(), key);
			App::GetInstance()->GetInputManager()->SetKeyChange(InputManager::KeyAction::NOKEY);
		}
		else
		{
			App::GetInstance()->GetInputManager()->m_ChangeKey = false;
		}
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			App::GetInstance()->GetFSM()->ChangeState(GameState::MENU);
		}
		break;
	case GameState::PAUSE:
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			App::GetInstance()->m_save.DealocateGameObjectMemory();
			App::GetInstance()->m_scene.GetRoot()->~Graph();
			App::GetInstance()->m_scene.InitScene();

			App::GetInstance()->GetFSM()->ChangeState(GameState::MENU);
		}
		break;
	case GameState::EDIT:
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			App::GetInstance()->GetFSM()->ChangeState(GameState::PLAY);
		}
		break;
	default:
		break;
	}
	
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (App::GetInstance()->m_wireframe == true)
		{
			App::GetInstance()->m_wireframe = false;
		}
		else
		{
			App::GetInstance()->m_wireframe = true;
		}
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		if (App::GetInstance()->m_active_mouse == true)
		{
			App::GetInstance()->m_active_mouse = false;
		}
		else
		{
			App::GetInstance()->m_active_mouse = true;
		}
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		if (App::GetInstance()->m_mouse_cursor == true)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			App::GetInstance()->m_mouse_cursor = false;
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			App::GetInstance()->m_mouse_cursor = true;
		}
	}
}

void mouse_callback(GLFWwindow*, double xpos, double ypos)
{
	if (App::GetInstance()->m_active_mouse)
	{
		if (App::GetInstance()->m_firstMouse)
		{
			App::GetInstance()->m_lastX = (float)xpos;
			App::GetInstance()->m_lastY = (float)ypos;
			App::GetInstance()->m_firstMouse = false;
		}

		float xoffset = (float)xpos - App::GetInstance()->m_lastX;
		float yoffset = App::GetInstance()->m_lastY - (float)ypos;

		App::GetInstance()->m_lastX = (float)xpos;
		App::GetInstance()->m_lastY = (float)ypos;

		App::GetInstance()->m_scene.m_camera.MouseMoveCamera(xoffset, yoffset);
	}
}


void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
	if (App::GetInstance()->m_scene.m_camera.m_fov > 1.0f && App::GetInstance()->m_scene.m_camera.m_fov < 45.0f)
		App::GetInstance()->m_scene.m_camera.m_fov -= (float)yoffset;
	else if (App::GetInstance()->m_scene.m_camera.m_fov <= 1.0f)
		App::GetInstance()->m_scene.m_camera.m_fov = 1.1f;
	else if (App::GetInstance()->m_scene.m_camera.m_fov >= 45.0f)
		App::GetInstance()->m_scene.m_camera.m_fov = 44.9f;
}
