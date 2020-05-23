#include "StatePlay.h"

//include Libgfx
#include "ResourceGFX.h" 
#include "Resources/Scene.h"
#include "Resources/LightManager.h"

#include "PhysicsWorld.h"

#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "PhysicsWorld.h"
#include "DebugDrawer.h"
#include "Save.h"
#include "App.h"

#pragma warning(push)
#pragma warning(push)
#pragma warning(disable:26495)
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#pragma warning(pop)

StatePlay::StatePlay()
{
											/*Init Lights*/
	//----------------------------------------------------------------------------------------------
	LowRenderer::LightManager::DirectionalLight({ -0.2f, -1.f, -0.3f }, { 0.2f, 0.2f, 0.2f }, { 0.5f, 0.5f, 0.5f }, { 1.f, 1.f, 1.f });
	LowRenderer::LightManager::PointLight({ 0.f, 0.f, 2.f }, { 1.f, 0.f, 0.f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f });
	LowRenderer::LightManager::SpotLight({ 0.5f, -6.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f });
	//----------------------------------------------------------------------------------------------

	/*Get the Light to configure*/
	LowRenderer::Light Spotlight = *LowRenderer::LightManager::GetLight(1);
	Spotlight.SetCutOff(12.5f, 17.5);
	Spotlight.SetDirection({ 0.f, 0.f, -1.f });

	/* Set the Light in LightManager */
	LowRenderer::LightManager::SetLight(Spotlight, 1);
	//----------------------------------------------------------------------------------------------

	/* Loop until the user closes the window */
	Core::Maths::vec3 beforeCamPos{ 0.f, 0.f, 0.f };
}

void StatePlay::OnBegin()
{
	m_currentState = GameState::PLAY;
	glfwSetInputMode(App::GetInstance()->m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!App::GetInstance()->m_gameInit)
	{
		Resources::Mesh mesh = Resources::Mesh::CreateCube();

		Resources::Mesh p("resource/sphere.obj");
		Resources::Mesh enemy("resource/Goomba.obj");


		Game::Player* player = new Game::Player(p.GetData(), App::GetInstance()->m_scene.GetRoot(), &(App::GetInstance()->m_scene.m_camera), "resource/rouge.jpg", 1, 100, { 0.0f, 8.0f, 0.0f }, { 0.5f, 0.5f, 0.5f }, { 0, 0, 0 });
		App::GetInstance()->m_save.SetPlayer(player);
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 0.0f, 0.0f, 0.0f }, { 7.5f, 1.0f, 7.5f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 0.0f, 0.0f, -9.0f }, { 5.0f, 1.0f, 5.0f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 3.0f, 1.0f, -17.0f }, { 6.0f, 1.0f, 6.0f }, { 20, 0, 0 }));
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 7.5f, 1.0f, -30.0f }, { 15.0f, 1.0f, 15.0f }, { 0, 0, 0 }));

		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(enemy, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { 7.5f, 3.0f, -30.0f }, { 0.5f, 0.5f, 0.5f }, { 0, 0, 0 }));

		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 17.5f, 2.f, -30.0f }, { 3.0f, 1.0f, 3.0f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 22.5f, 3.f, -30.0f }, { 3.0f, 1.0f, 3.0f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 27.5f, 4.f, -30.0f }, { 3.0f, 1.0f, 3.0f }, { 0, 0, 0 }));


		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 35.0f, 2.f, -30.0f }, { 2.0f, 10.0f, 9.0f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 38.0f, -5.5f, -30.0f }, { 7.5f, 1.0f, 7.5f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 47.5f, -4.5f, -14.0f }, { 20.0f, 1.0f, 20.0f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 47.5f, -3.5f, 8.0f }, { 20.0f, 1.0f, 20.0f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddGameObject(new Game::GameObject(mesh.GetData(), App::GetInstance()->m_scene.GetRoot(), "resource/vert.jpg", 0, { 32.0f, -2.5f, 8.0f }, { 8.0f, 1.0f, 8.0f }, { 0, 0, 0 }));

		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(enemy, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { 47.5f, 1.5f, -14.0f }, { 0.5f, 0.5f, 0.5f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(enemy, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { 45.5f, 4.5f, -8.0f }, { 0.5f, 0.5f, 0.5f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(enemy, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { 42.5f, 0.5f, -20.0f }, { 0.5f, 0.5f, 0.5f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(enemy, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { 50.0f, 2.5f, -10.0f }, { 0.5f, 0.5f, 0.5f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(enemy, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { 41.5f, 7.f, -18.0f }, { 0.5f, 0.5f, 0.5f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(enemy, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { 39.0f, 7.f, -12.0f }, { 0.5f, 0.5f, 0.5f }, { 0, 0, 0 }));

		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(enemy, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { 49.5f, 7.f, 8.0f }, { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(enemy, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { 47.5f, 6.f, 3.0f }, { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 }));
		App::GetInstance()->m_save.AddEnemy(new Game::Enemy(enemy, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { 45.5f, 8.f, 14.0f }, { 1.0f, 1.0f, 1.0f }, { 0, 0, 0 }));

		App::GetInstance()->m_gameInit = true;
	} 	
}


GameState StatePlay::OnUpdate(float deltaTime)
{
	//new framme IMGUI
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	App::GetInstance()->GetDebugDrawer()->SetMatrices(App::GetInstance()->m_scene.GetViewMatrix(), App::GetInstance()->m_scene.GetProjectionMatrix());
	PhysicsWorld::GetInstance()->UpdateDynamicWorld(deltaTime);

	App::GetInstance()->m_save.UpdateGameObjects(deltaTime);

	//--- UI Life Player ---//
	DisplayLifePlayer();

	/*Draw Wireframe*/
	WireFrameGFX(App::GetInstance()->m_wireframe);

	/*Scene Update Functions*/
	App::GetInstance()->m_scene.UpdateScene(deltaTime);
	App::GetInstance()->m_scene.RenderScene();

	//TODO Felix
	PhysicsWorld::GetInstance()->DrawWorld();
	

	/*Set Material*/
	//----------------------------------------------------------------------------------------------
	//http://devernay.free.fr/cours/opengl/materials.html /*READ...:)*/
	App::GetInstance()->m_scene.m_shader.SetVec3({ 1.f, 1.f, 1.f }, "material.ambient", App::GetInstance()->m_scene.m_shader.GetID());

	App::GetInstance()->m_scene.m_shader.SetVec3({ 1.0f, 0.5f, 0.31f }, "material.diffuse", App::GetInstance()->m_scene.m_shader.GetID());

	App::GetInstance()->m_scene.m_shader.SetVec3({ 0.5f, 0.5f, 0.5f }, "material.specular", App::GetInstance()->m_scene.m_shader.GetID());

	App::GetInstance()->m_scene.m_shader.SetFloat(32.0f, "material.shininess", App::GetInstance()->m_scene.m_shader.GetID());
	//----------------------------------------------------------------------------------------------

	/*SpotLight*/
	//-----------------------------------------------------------------------------------------------
	/*flashlight configuration*/
	//Spotlight.SetPosition({ g_scene.m_camera.m_cameraPos.m_x,g_scene.m_camera.m_cameraPos.m_y,g_scene.m_camera.m_cameraPos.m_z, 1.f });
	//Spotlight.SetDirection(g_scene.m_camera.m_cameraFront);

	/*Light Color (Test color variadic)*/
	/*
	Core::Maths::vec3 lightColor{ 0.f, 0.f, 0.f };
	lightColor.m_x = sinf((float)glfwGetTime() * 2.0f);
	lightColor.m_y = sinf((float)glfwGetTime() * 0.7f);
	lightColor.m_z = sinf((float)glfwGetTime() * 1.3f);
	Core::Maths::vec3 diffuseColor = lightColor * Core::Maths::vec3(0.5f, 0.5f, 0.5f); // decrease the influence
	Core::Maths::vec3 ambientColor = diffuseColor * Core::Maths::vec3(0.2f, 0.2f, 0.2f); // low influence
	Spotlight.SetAmbientColor(ambientColor);
	Spotlight.SetDiffuseColor(diffuseColor);
	*/
	/*Static color Light*/
	//Spotlight.SetAmbientColor({ 0.1f, 0.1f, 0.1f });
	//Spotlight.SetDiffuseColor({ 0.8f, 0.8f, 0.8f });

	/* Set the Light in LightManager *///Careful to pass the good index XD
	//LowRenderer::LightManager::SetLight(Spotlight, 1);
	//----------------------------------------------------------------------------------------------


	

	/*Update light values in Shader*/
	LowRenderer::LightManager::UpdateShader(App::GetInstance()->m_scene.m_shader.GetID());


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	if (App::GetInstance()->m_save.GetPlayer()->GetLife() <= 0)
	{
		App::GetInstance()->m_save.DealocateGameObjectMemory();
		App::GetInstance()->m_scene.GetRoot()->~Graph();
		App::GetInstance()->m_scene.InitScene();
		App::GetInstance()->m_save.ImportSave(App::GetInstance()->m_scene.GetRoot(), &(App::GetInstance()->m_scene.m_camera));
		App::GetInstance()->m_gameInit = true;
	}



	return m_currentState;
}


void StatePlay::DisplayLifePlayer()
{
	bool life = true;
	ImGui::SetNextWindowPos(ImVec2(-5, -2));
	ImGui::SetNextWindowSize({ (float)App::GetInstance()->m_windowWidth + 20, (float)App::GetInstance()->m_windowHeight + 20 });

	ImGui::Begin("PlayerLife2", &life, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);

	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 13, (float)App::GetInstance()->m_windowHeight / 17));
	ImGui::Text("Player Life:");
	ImGui::SetCursorPos(ImVec2((float)App::GetInstance()->m_windowWidth / 13, (float)App::GetInstance()->m_windowHeight / 12));
	ImGui::ProgressBar((float)Game::Player::GetInstance()->GetLife() / 100, { 250,30 });
	ImGui::End();
}


void StatePlay::OnEnd()
{

}

