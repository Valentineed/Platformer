#include "Enemy.h"
#include "Resources/ResourcesManager.h"

#include "App.h"

Game::Enemy::Enemy(Resources::Mesh& mesh, Core::DataStructure::Graph* root, Game::Player* player, std::string filePath, float mass, Core::Maths::vec3 position, Core::Maths::vec3 scale, Core::Maths::vec3 rotation)
{
	LowRenderer::Model* model = new LowRenderer::Model(mesh.GetData());
	unsigned int texture = Resources::ResourcesManager::GetTexture(filePath);
	model->SetTextureID(texture);
	m_objectGraphic = new Core::DataStructure::Graph(model, Core::Maths::vec4(1, 1, 1, 1));

	m_player = player;
	m_scale = scale;
	m_position = position;
	m_rotation = ConvertEulerToQuaternion(rotation.m_x, rotation.m_y, rotation.m_z);
	m_scaleBoxCollider = model->GetScaleBox() * m_scale;

	Core::Maths::mat4 transform = Resources::Transform::CreateTranslateMatrix(m_position);
	transform = Resources::Transform::Rotate(transform, rotation.m_x, { 1, 0, 0 });
	transform = Resources::Transform::Rotate(transform, rotation.m_y, { 0, 1, 0 });
	transform = Resources::Transform::Rotate(transform, rotation.m_z, { 0, 0, 1 });

	m_objectGraphic->SetTransform(transform);
	m_objectGraphic->SetModelScale(m_scale);

	root->AddChild(m_objectGraphic);

	m_objectPhysic = new RigidBody({ m_rotation.m_x, m_rotation.m_y, m_rotation.m_z, m_rotation.m_w }, { m_position.m_x, m_position.m_y, m_position.m_z }, { m_scaleBoxCollider.m_x, m_scaleBoxCollider.m_y, m_scaleBoxCollider.m_z }, mass, { 0, 0, 0 });
	m_objectPhysic->FreezeRotation();

	m_spawnPoint = position;
}

void Game::Enemy::Update(float deltaTime)
{
	UpdateTransform();

	if (m_isDead)
	{
		m_timer -= deltaTime;
		if (m_timer <= 0)
		{
			m_objectPhysic->SetPosition({ m_spawnPoint.m_x, m_spawnPoint.m_y, m_spawnPoint.m_z });
			m_objectPhysic->ChangeGravity(1);
			m_isDead = false;
		}

		return;
	}

	if (m_position.m_y <= -50)
	{
		m_isDead = true;
		m_timer = m_timeToRespawn;

		m_objectPhysic->Translate({ 1000, 0, 0 });
		m_objectPhysic->ChangeGravity(0);
	}

	if (CheckDistanceTarget())
	{
		TargetPlayer(deltaTime);
	}
	else
	{
		StandardMove(deltaTime);
	}

	CheckCollisions();
}

void Game::Enemy::StandardMove(float deltaTime)
{
	if (!m_objectPhysic->isGrounded())
	{
		return;
	}

	float moveSpeed = 1 * deltaTime;

	m_positionInCircle += moveSpeed;

	float x = sinf(m_positionInCircle) * m_circleRadius;
	float z = cosf(m_positionInCircle) * m_circleRadius;

	m_objectPhysic->Translate({ x, 0, z });
}

void Game::Enemy::TargetPlayer(float deltaTime)
{
	float moveSpeed = 3 * deltaTime;

	Core::Maths::vec3 dir = m_player->GetPosition() - m_position;
	dir.Normalize();

	m_objectPhysic->Translate({ dir.m_x * moveSpeed, 0, dir.m_z * moveSpeed });
}

bool Game::Enemy::CheckDistanceTarget()
{
	Core::Maths::vec3 distance = m_player->GetPosition() - m_position;

	if (distance.GetMagnitude() < m_distanceTarget)
	{
		return true;
	}

	return false;
}

void Game::Enemy::CheckCollisions()
{
	if (m_objectPhysic->CheckSideCollisionWith(m_player->GetRigidBody()))
	{
		m_player->TakeDamage(10);
	}

	if (m_objectPhysic->CheckUpCollisionWith(m_player->GetRigidBody()))
	{
		m_isDead = true;
		m_timer = m_timeToRespawn;

		m_objectPhysic->Translate({ 1000, 0, 0 });
		m_objectPhysic->ChangeGravity(0);
	}
}

