#include "Save.h"

#include "GameObject.h"
#include "Resources/Mesh.h"
#include "DataStructure/Graph.h"
#include "App.h"

#include <vector>
#include <iostream> 
#include <fstream> 

Game::Player* Save::GetPlayer()
{
	return m_playerObject;
}

void Save::ImportSave(Core::DataStructure::Graph* root, LowRenderer::Camera* camera)
{
	std::ifstream saveFile;

	saveFile.open("Save.bin", std::ios::in);

	PlayerToSave player;
	ObjectToSave obj;

	Resources::Mesh cube = Resources::Mesh::CreateCube();
	Resources::Mesh sphere("resource/sphere.obj");
	Resources::Mesh enemyMesh("resource/Goomba.obj");

	saveFile.read((char*)&player, sizeof(player));

	SetPlayer(new Game::Player(sphere.GetData(), root, camera, "resource/rouge.jpg", 1, player.m_hp, { player.m_position.getX(), player.m_position.getY(), player.m_position.getZ() }, { 0.5f, 0.5f, 0.5f }, { player.m_rotation.getX(), player.m_rotation.getY(), player.m_rotation.getZ() }));

	saveFile.read((char*)&obj, sizeof(obj));
	while (!saveFile.eof()) 
	{
		AddGameObject(new Game::GameObject({ cube.GetData() }, root, "resource/vert.jpg", 0, { obj.m_position.getX(), obj.m_position.getY(), obj.m_position.getZ() }, { obj.m_scale.getX(), obj.m_scale.getY(), obj.m_scale.getZ() }, { obj.m_rotation.getX(), obj.m_rotation.getY(), obj.m_rotation.getZ() }));
		saveFile.read((char*)&obj, sizeof(obj));
	}
	saveFile.close();

	std::ifstream enemyFile;

	enemyFile.open("Enemies.bin", std::ios::in);

	EnemyToSave enemy;

	enemyFile.read((char*)&enemy, sizeof(enemy));
	while (!enemyFile.eof())
	{
		AddEnemy(new Game::Enemy(enemyMesh, App::GetInstance()->m_scene.GetRoot(), App::GetInstance()->m_save.GetPlayer(), "resource/vert.jpg", 1, { enemy.m_position.getX(), enemy.m_position.getY(), enemy.m_position.getZ() }, { 0.5f, 0.5f, 0.5f }, { obj.m_rotation.getX(), obj.m_rotation.getY(), obj.m_rotation.getZ() }));
		enemyFile.read((char*)&enemy, sizeof(enemy));
	}

	enemyFile.close();
	App::GetInstance()->m_gameInit = true;
}

void Save::ExportSave() const
{
	std::ofstream saveFile;

	saveFile.open("Save.bin", std::ofstream::trunc);

	{
		PlayerToSave player;

		player.m_hp = 100;
		player.m_position = { m_playerObject->GetPosition().m_x, m_playerObject->GetPosition().m_y,m_playerObject->GetPosition().m_z };
		player.m_rotation = { m_playerObject->GetRotation().m_x, m_playerObject->GetRotation().m_y, m_playerObject->GetRotation().m_z, m_playerObject->GetRotation().m_w };
		saveFile.write((char*)&player, sizeof(player));
	}

	{
		ObjectToSave obj;

		for (Game::GameObject* gameObject : m_gameObjects)
		{
			obj.m_position = { gameObject->GetPosition().m_x, gameObject->GetPosition().m_y,gameObject->GetPosition().m_z };
			obj.m_scale = { gameObject->GetScale().m_x, gameObject->GetScale().m_y,gameObject->GetScale().m_z };
			obj.m_rotation = { gameObject->GetRotation().m_x, gameObject->GetRotation().m_y, gameObject->GetRotation().m_z, gameObject->GetRotation().m_w };
			saveFile.write((char*)&obj, sizeof(obj));
		}
	}
	saveFile.close();

	std::ofstream enemyFile;

	enemyFile.open("Enemies.bin", std::ofstream::trunc);
	{
		EnemyToSave enemy;

		for (Game::Enemy* enemyObject : m_enemies)
		{
			enemy.m_position = { enemyObject->GetPosition().m_x, enemyObject->GetPosition().m_y,enemyObject->GetPosition().m_z };
			enemy.m_rotation = { enemyObject->GetRotation().m_x, enemyObject->GetRotation().m_y, enemyObject->GetRotation().m_z, enemyObject->GetRotation().m_w };
			enemyFile.write((char*)&enemy, sizeof(enemy));
		}
	}
	enemyFile.close();
}

void Save::SetPlayer(Game::Player* player)
{
	if (player == nullptr)
	{
		return;
	}
	m_playerObject = player;
}

void Save::AddEnemy(Game::Enemy* enemyPtr)
{
	if (enemyPtr == nullptr)
	{
		return;
	}
	m_enemies.push_back(enemyPtr);
}

std::vector<Game::GameObject*> Save::GetObjects() const
{
	return m_gameObjects;
}

std::vector<Game::Enemy*> Save::GetEnemy() const
{
	return m_enemies;
}

void Save::AddGameObject(Game::GameObject* gameObjectPtr)
{
	if (gameObjectPtr == nullptr)
	{
		return;
	}

	m_gameObjects.push_back(gameObjectPtr);
}



void Save::UpdateGameObjects(float deltaTime)
{
	m_playerObject->Update(deltaTime);
	for (Game::GameObject* gameObject : m_gameObjects)
	{
		gameObject->Update(deltaTime);
	}

	for (Game::Enemy* enemy : m_enemies)
	{
		enemy->Update(deltaTime);
	}
}

void Save::DealocateGameObjectMemory()
{
	DeletePlayer();
	DeleteEnemies();
	DeleteGameObjects();
}

void Save::DeletePlayer()
{
	delete m_playerObject;
}

void Save::DeleteGameObjects()
{
	for (Game::GameObject* gameObject: m_gameObjects)
	{
		delete gameObject;
	}
	m_gameObjects.clear();
}

void Save::DeleteEnemies()
{
	for (Game::Enemy* enemy : m_enemies)
	{
		delete enemy;
	}
	m_enemies.clear();
}
