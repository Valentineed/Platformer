#include "StateEdit.h"

#pragma warning(push)
#pragma warning(disable:26495)
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#pragma warning(pop)
#include "App.h"
#include "Maths/vec3.h"
#include <string>
#include "PhysicsWorld.h"

void StateEdit::OnBegin()
{
	glfwSetInputMode(App::GetInstance()->m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	UpdateVectorPlatform();
	UpdateVectorEnemy();
	m_gameObject = 0;
	m_enemy = 0;
}

GameState StateEdit::OnUpdate(float deltaTime)
{
	GameState nextState = GameState::EDIT;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//Set Window
	bool editor = true;
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2((float)App::GetInstance()->m_windowWidth / 4, (float)App::GetInstance()->m_windowHeight));
	ImGui::SetNextWindowBgAlpha(0.02f);
	ImGui::Begin("Editor", &editor, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

	//Reset Translate
	m_platformX = 0, m_platformY = 0, m_platformZ = 0;
	m_playerX = 0, m_playerY = 0, m_playerZ = 0;
	m_enemyX = 0, m_enemyY = 0, m_enemyZ = 0;

	PlatformButton();
	PlayerButton();
	EnemyButton();

	if (ImGui::Button("Quit", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		nextState = GameState::PLAY;
	}

	UpdatePosition();

	/*Scene Update Functions*/
	App::GetInstance()->m_scene.UpdateScene(deltaTime);
	App::GetInstance()->m_scene.RenderScene();
	PhysicsWorld::GetInstance()->DrawWorld();
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	return nextState;
}

void StateEdit::UpdatePosition()
{
	//Translate Objects
	App::GetInstance()->m_save.GetObjects()[m_gameObject]->GetRigidBody()->Translate({ m_platformX, m_platformY, m_platformZ });
	App::GetInstance()->m_save.GetEnemy()[m_enemy]->GetRigidBody()->Translate({ m_enemyX, m_enemyY, m_enemyZ });
	App::GetInstance()->m_save.GetPlayer()->GetRigidBody()->Translate({ m_playerX, m_playerY, m_playerZ });

	//Update Objects
	App::GetInstance()->m_save.GetPlayer()->UpdateTransform();
	App::GetInstance()->m_save.GetObjects()[m_gameObject]->UpdateTransform();
	App::GetInstance()->m_save.GetEnemy()[m_enemy]->UpdateTransform();
	//App::GetInstance()->m_save.UpdateGameObjects(deltaTime);
}

bool StateEdit::String_getter(void* data, int idx, const char** out_text)
{
	std::string* string_arrray = static_cast<std::string*>(data);
	*out_text = string_arrray[idx].c_str();
	return true;
}

void StateEdit::PlatformButton()
{
	ImGui::Combo("Platforms", &m_gameObject, String_getter, static_cast<void*>(&m_platfoms[0]), (int)App::GetInstance()->m_save.GetObjects().size());
	if (ImGui::Button("Change X Right", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_platformX = 2.f;
	}
	if (ImGui::Button("Change X Left", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_platformX = -2.f;
	}
	if (ImGui::Button("Change Y Up", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_platformY = 2.f;
	}
	if (ImGui::Button("Change Y Down", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_platformY = -2.f;
	}
	if (ImGui::Button("Change Z Front", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_platformZ = 2.f;
	}
	if (ImGui::Button("Change Z Back", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_platformZ = -2.f;
	}
	if (ImGui::Button("Create Platforme", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		Resources::Mesh mesh = Resources::Mesh::CreateCube();
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 0, 0, 0.0f }, { 25, 0.5, 25 }, { 0, 0, 0 }));
		UpdateVectorPlatform();
	}
}

void StateEdit::PlayerButton()
{
	if (ImGui::Button("Player move left", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_playerX = -2.f;
	}
	if (ImGui::Button("Player move Right", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_playerX = 2.f;
	}
	if (ImGui::Button("Player move Up", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_playerY = 2.f;
	}
	if (ImGui::Button("Player move Down", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_playerY = -2.f;
	}
	if (ImGui::Button("Player move Back", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_playerZ = -2.f;
	}
	if (ImGui::Button("Player move Front", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_playerZ = 2.f;
	}
}

void StateEdit::EnemyButton()
{
	ImGui::Combo("Enemies", &m_enemy, String_getter, static_cast<void*>(&m_enemiesArray[0]), (int)App::GetInstance()->m_save.GetEnemy().size());
	if (ImGui::Button("Enemy move left", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_enemyX = -2.f;
	}
	if (ImGui::Button("Enemy move Right", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_enemyX = 2.f;
	}
	if (ImGui::Button("Enemy move Up", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_enemyY = 2.f;
	}
	if (ImGui::Button("Enemy move Down", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_enemyY = -2.f;
	}
	if (ImGui::Button("Enemy move Back", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_enemyZ = -2.f;
	}
	if (ImGui::Button("Enemy move Front", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		m_enemyZ = 2.f;
	}
	if (ImGui::Button("Create Enemy", ImVec2((float)App::GetInstance()->m_windowWidth / 8.333f, (float)App::GetInstance()->m_windowHeight / 30.f)))
	{
		Resources::Mesh p("resource/sphere.obj");
		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(p, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { .0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f }, { 0, 0, 0 }));
		UpdateVectorEnemy();
	}
}

void StateEdit::UpdateVectorPlatform()
{
	size_t size = App::GetInstance()->m_save.GetObjects().size();
	if (size != 0)
	{
		for (size_t i = 0; i < size; i++)
		{
			m_platfoms[i] = "Platforme ";
			m_platfoms[i] += std::to_string(i);
		}
	}
}

void StateEdit::UpdateVectorEnemy()
{
	size_t size = App::GetInstance()->m_save.GetEnemy().size();
	if (size != 0)
	{
		for (size_t i = 0; i < size; i++)
		{
			m_enemiesArray[i] = "Enemy ";
			m_enemiesArray[i] += std::to_string(i);
		}
	}
}

void StateEdit::OnEnd()
{
}