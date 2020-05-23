#pragma once

#include "GameObject.h"
#include "Player.h"
#include "Maths/vec4.h"

namespace Game
{
	class Enemy : public GameObject
	{
	public:

		Enemy(Resources::Mesh& mesh, Core::DataStructure::Graph* root, Game::Player* player, std::string filePath, float mass = 1, Core::Maths::vec3 position = { 0, 0, 0 }, Core::Maths::vec3 scale = { 1, 1, 1 }, Core::Maths::vec3 rotation = { 0, 0, 0 });
		~Enemy() = default;

		void Update(float deltaTime) override;

		void StandardMove(float deltaTime);
		void TargetPlayer(float deltaTime);
		bool CheckDistanceTarget();
		void CheckCollisions();



	private:

		Player* m_player;

		Core::Maths::vec3 m_spawnPoint = { 0, 0, 0 };

		bool m_isDead = false;
		float m_timeToRespawn = 10;
		float m_timer = 0;

		float m_distanceTarget = 7.5f;

		float m_circleRadius = 0.05f;
		float m_positionInCircle = 0;

	};
}

