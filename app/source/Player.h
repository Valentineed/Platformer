#pragma once
#include "GameObject.h"
#include "Maths/vec4.h"
#include "DataStructure/Graph.h"
#include "RigidBody.h"
#include "LowRenderer/Camera.h"

namespace Game
{

	enum class PlayerMove
	{
		MOVE_BACKWARD,
		MOVE_FORWARD,
		MOVE_LEFT,
		MOVE_RIGHT
	};


	class Player : public GameObject
	{
	public:

		Player(const std::vector<float>& meshData, Core::DataStructure::Graph* root, LowRenderer::Camera* cam, std::string filePath, float mass = 1, int hp = 100, Core::Maths::vec3 position = { 0, 0, 0 }, Core::Maths::vec3 scale = { 1, 1, 1 }, Core::Maths::vec3 rotation = { 0, 0, 0 });
		~Player() =  default;

		void Update(float deltaTime) override;

		void Move(PlayerMove moveState);
		void Jump();

		static Player* GetInstance();

		int GetLife();
		void TakeDamage(int damage);

	private:
		LowRenderer::Camera* m_camera;

		static Player* ms_instance;
		bool m_isJump = false;

		float m_timeInvincibility = 4;
		float m_timerInvincibility = 0;

		int m_hp = 100;
	};

}

