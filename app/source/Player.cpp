#include "Player.h"
#include "Maths/vec4.h"
#include "DataStructure/Graph.h"
#include "RigidBody.h"
#include "Resources/ResourcesManager.h"
#include "Physics/Transform.h"
#include "App.h"

Game::Player* Game::Player::ms_instance = nullptr;

Game::Player::Player(const std::vector<float>& dataMesh, Core::DataStructure::Graph* root, LowRenderer::Camera* cam, std::string filePath, float mass, int hp, Core::Maths::vec3 position, Core::Maths::vec3 scale, Core::Maths::vec3 rotation)
{
	ms_instance = this;

	LowRenderer::Model* model = new LowRenderer::Model(dataMesh);
	unsigned int texture = Resources::ResourcesManager::GetTexture(filePath);
	model->SetTextureID(texture);
	m_objectGraphic = new Core::DataStructure::Graph(model, Core::Maths::vec4(1, 1, 1, 1));

	m_hp = hp;
	m_scale = scale;
	m_position = position;
	m_rotation = ConvertEulerToQuaternion(rotation.m_x, rotation.m_y, rotation.m_z);
	m_scaleBoxCollider = model->GetScaleBox() * m_scale;

	m_camera = cam;
	m_camera->SetIsLookingAt(true);
	m_camera->SetTarget(m_position);

	m_objectGraphic->SetTransform(Resources::Transform::CreateTranslateMatrix(m_position));
	m_objectGraphic->SetModelScale(m_scale);

	root->AddChild(m_objectGraphic);

	m_objectPhysic = new RigidBody(m_scale.m_x, { m_position.m_x, m_position.m_y, m_position.m_z }, mass, { 0, 0, 0 });
	m_objectPhysic->FreezeRotation();
}

void Game::Player::Update(float deltaTime)
{
	if (m_timerInvincibility > 0)
	{
		m_timerInvincibility -= deltaTime;
	}

	if (m_position.m_y <= -50)
	{
		m_hp = 0;
	}

	UpdateTransform();
	//m_camera->m_cameraPos = Core::Maths::vec3{ m_position.m_x, m_position.m_y , m_position.m_z + 10 };
	m_camera->SetTarget(m_position);

	if (m_isJump && m_objectPhysic->isGrounded())
	{
		m_isJump = false;
	}

	if (m_objectPhysic->isGliding())
	{
		m_objectPhysic->ChangeGravity(0.25f);
	}
	else
	{
		m_objectPhysic->ChangeGravity(1);
	}
}

void Game::Player::Move(PlayerMove moveState)
{
	float moveSpeed = 5.f * App::GetInstance()->GetTime();
	Core::Maths::vec3 front = m_camera->m_cameraFront.Normalize();
	Core::Maths::vec3 right = m_camera->m_cameraRight.Normalize();

	switch (moveState)
	{
		case PlayerMove::MOVE_BACKWARD :
		{ m_objectPhysic->Translate({ front.m_x * -moveSpeed, 0, front.m_z * -moveSpeed }); }
		break;
		case PlayerMove::MOVE_FORWARD:
		{ m_objectPhysic->Translate({ front.m_x * moveSpeed, 0, front.m_z * moveSpeed }); }
		break;
		case PlayerMove::MOVE_LEFT:
		{ m_objectPhysic->Translate({ right.m_x * moveSpeed, 0, right.m_z * moveSpeed }); }
		break;
		case PlayerMove::MOVE_RIGHT:
		{ m_objectPhysic->Translate({ right.m_x * -moveSpeed, 0, right.m_z * -moveSpeed }); }
		break;
	}
}

void Game::Player::Jump()
{
	if (!m_isJump && m_objectPhysic->isGrounded())
	{
		m_objectPhysic->AddForce({ 0, 175 , 0 });
		m_isJump = true;
	}
}

Game::Player* Game::Player::GetInstance()
{
	return ms_instance;
}

int Game::Player::GetLife()
{
	return m_hp;
}

void Game::Player::TakeDamage(int damage)
{
	if (m_hp > 0 && m_timerInvincibility <= 0)
	{
		m_hp -= damage;
		m_timerInvincibility = m_timeInvincibility;
	}
}

