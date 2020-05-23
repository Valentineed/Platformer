#pragma once

#include <vector>

#include "Vec3.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"

struct PlayerToSave
{
	int m_hp = 0;
	Vec3 m_position;
	Quaternion m_rotation;
};

struct ObjectToSave
{
	Vec3 m_position;
	Vec3 m_scale;
	Quaternion m_rotation;
};

struct EnemyToSave
{
	Vec3 m_position;
	Quaternion m_rotation;
};

class Save
{
public:
	Save() = default;
	Save(const Save& other) = delete;
	~Save() = default;
	Save& operator=(const Save& other) = delete;

	Game::Player* GetPlayer();

	void ImportSave(Core::DataStructure::Graph* root, LowRenderer::Camera* camera);
	void ExportSave() const;
	
	void SetPlayer(Game::Player* player);
	void AddGameObject(Game::GameObject* gameObjectPtr);
	void AddEnemy(Game::Enemy* enemyPtr);

	std::vector<Game::GameObject*> GetObjects() const;
	std::vector<Game::Enemy*> GetEnemy() const;

	void UpdateGameObjects(float deltaTime);
	void DealocateGameObjectMemory();
private:
	void DeletePlayer();
	void DeleteGameObjects();
	void DeleteEnemies();

	Game::Player* m_playerObject = nullptr;
	std::vector<Game::GameObject*> m_gameObjects;
	std::vector<Game::Enemy*> m_enemies;
};
