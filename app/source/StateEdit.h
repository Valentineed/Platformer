#pragma once
#include "State.h"
#include <string>
class StateEdit : public State
{
public:
	StateEdit() = default;
	~StateEdit() = default;

	GameState GetState() const override { return GameState::EDIT; }
	virtual GameState OnUpdate(float deltaTime) override;
	virtual void OnEnd() override;
	void OnBegin() override;
private:
	//UpdatePosition
	void UpdatePosition();

	//float for Config
	float m_platformX = 0, m_platformY = 0, m_platformZ = 0;
	float m_playerX = 0, m_playerY = 0, m_playerZ = 0;
	float m_enemyX = 0, m_enemyY = 0, m_enemyZ = 0;

	//ImGui
	void PlatformButton();
	void PlayerButton();
	void EnemyButton();

	static bool String_getter(void* data, int idx, const char** out_text);

	//List
	void UpdateVectorPlatform();
	void UpdateVectorEnemy();
	int m_gameObject = 0;
	std::string m_platfoms[64];
	int m_enemy = 0;
	std::string m_enemiesArray[64];

};