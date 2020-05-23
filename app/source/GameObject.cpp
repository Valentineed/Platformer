#include "GameObject.h"
#include"Maths/mat4.h"
#include "Resources/ResourcesManager.h"
#include "LowRenderer/Model.h"
#include "Physics/Transform.h"
#include "Quaternion.h"


Game::GameObject::GameObject(const std::vector<float>& meshData, Core::DataStructure::Graph* root, std::string filePath, float mass, Core::Maths::vec3 position, Core::Maths::vec3 scale, Core::Maths::vec3 rotation)
{
	m_meshData = meshData;
	m_mass = mass;
	m_texturePath = filePath;

	LowRenderer::Model* model = new LowRenderer::Model(meshData);
	unsigned int texture = Resources::ResourcesManager::GetTexture(filePath);
	model->SetTextureID(texture);
	m_objectGraphic =  new Core::DataStructure::Graph (model, Core::Maths::vec4(1, 1, 1, 1));

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
}

Game::GameObject::~GameObject()
{
	m_objectPhysic->RemoveRigidBody();
}

void Game::GameObject::UpdateTransform()
{

	float* values = m_objectPhysic->GetTransform();
	m_position.m_x = values[0];
	m_position.m_y = values[1];
	m_position.m_z = values[2];

	m_rotation = { values[3], values[4], values[5], 1 };

	Core::Maths::vec3 angle(values[3] * (180/3.14f), values[4] * (180 / 3.14f), values[5] * (180 / 3.14f));
	m_rotation = { angle.m_z, angle.m_y, angle.m_x, 1 };

	Core::Maths::mat4 transform = Resources::Transform::CreateTranslateMatrix(m_position);
	transform = Resources::Transform::Rotate(transform, angle.m_x, {1, 0, 0});
	transform = Resources::Transform::Rotate(transform, angle.m_y, { 0, 1, 0 });
	transform = Resources::Transform::Rotate(transform, angle.m_z, { 0, 0, 1 });

	m_objectGraphic->SetTransform(transform);
}

void Game::GameObject::Update(float deltaTime)
{
	deltaTime = deltaTime;
	UpdateTransform();
}

Core::Maths::vec3 Game::GameObject::GetPosition()
{
	return m_position;
}

const Core::Maths::vec3& Game::GameObject::GetScale() const
{
	return m_scale;
}

const Core::Maths::vec4& Game::GameObject::GetRotation() const
{
	return m_rotation;
}

const float Game::GameObject::GetMass() const
{
	return m_mass;
}

const std::vector<float>& Game::GameObject::GetMeshData() const
{
	return m_meshData;
}

const std::string& Game::GameObject::GetTexturePath() const
{
	return m_texturePath;

}

RigidBody* Game::GameObject::GetRigidBody()
{
	return m_objectPhysic;
}

Core::Maths::vec4 Game::GameObject::ConvertEulerToQuaternion(float x, float y, float z)
{
	x = x * (3.14f / 180);
	y = y * (3.14f / 180);
	z = z * (3.14f / 180);

	float qx = sinf(z / 2) * cosf(y / 2) * cosf(x / 2) - cosf(z / 2) * sinf(y / 2) * sinf(x / 2);
	float qy = cosf(z / 2) * sinf(y / 2) * cosf(x / 2) + sinf(z / 2) * cosf(y / 2) * sinf(x / 2);
	float qz = cosf(z / 2) * cosf(y / 2) * sinf(x / 2) - sinf(z / 2) * sinf(y / 2) * cosf(x / 2);
	float qw = cosf(z / 2) * cosf(y / 2) * cosf(x / 2) + sinf(z / 2) * sinf(y / 2) * sinf(x / 2);

	return { qx, qy, qz, qw };
}

Core::Maths::vec3 Game::GameObject::ConverQuaternionToEuler(float x, float y, float z, float w)
{
	float t0 = 2.0f * ((w * x) + (y * z));
	float t1 = 1.0f - 2.0f * ((x * x) + (y * y));
	float roll = atan2f(t0, t1);
	float t2 = 2.0f * ((w * y) - (z * x));

	if (t2 > 1.0f)
	{
		t2 = 1.0f;
	}
	
	if (t2 > -1.0f)
	{
		t2 = -1.0f;
	}

	float pitch = asinf(t2);
	float t3 = 2.0f * ((w * z) + (x * y));
	float t4 = 1.0f - 2.0f * ((y * y) + (z * z));
	float yaw = atan2f(t3, t4);
	return { yaw, pitch, roll };
}


