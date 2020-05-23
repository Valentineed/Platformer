#pragma once

#include "Maths/vec3.h"
#include "Maths/vec4.h"
#include "DataStructure/Graph.h"
#include "RigidBody.h"

namespace Game
{

	class GameObject
	{
	public:

		GameObject() = default;
		~GameObject();
		GameObject(const std::vector<float>& meshData, Core::DataStructure::Graph* root, std::string filePath, float mass = 0, Core::Maths::vec3 position = { 0, 0, 0 }, Core::Maths::vec3 scale = { 1, 1, 1 }, Core::Maths::vec3 rotation = {0, 0, 0});

		void UpdateTransform();
		virtual void Update(float deltaTime);

		RigidBody* GetRigidBody();

		Core::DataStructure::Graph* GetGraph() { return m_objectGraphic; };

		Core::Maths::vec3 GetPosition();
		const Core::Maths::vec3& GetScale() const;
		const Core::Maths::vec4& GetRotation() const;
		const float GetMass() const;
		const std::vector<float>& GetMeshData() const;
		const std::string& GetTexturePath() const;


	protected:
		

		Core::Maths::vec4 ConvertEulerToQuaternion(float x, float y, float z);
		Core::Maths::vec3 ConverQuaternionToEuler(float x, float y, float z, float w);

		Core::Maths::vec3 m_scaleBoxCollider = Core::Maths::vec3{ 1, 1, 1 };
		Core::Maths::vec3 m_scale = Core::Maths::vec3{ 0, 0, 0 };
		Core::Maths::vec3 m_position = Core::Maths::vec3{0, 0, 0};
		Core::Maths::vec4 m_rotation = Core::Maths::vec4{ 0, 0, 0, 1};
		std::string m_texturePath = "Not set";
		float m_mass = 0.0f;
		std::vector<float> m_meshData;

		RigidBody* m_objectPhysic;
		Core::DataStructure::Graph* m_objectGraphic;
	};

}

