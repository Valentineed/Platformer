#pragma once
#include "State.h"
#include "Maths/vec4.h"
#include "InputManager.h"
class StateOption : public State
{
public:
	StateOption() = default;
	~StateOption() = default;

	GameState GetState() const override { return GameState::SETTINGS; }
	virtual GameState OnUpdate(float deltaTime) override;
	virtual void OnEnd() override;
	void OnBegin() override;
	static void PopWindow();
private:
	int m_mattisX = 250;
	static bool m_popWindow;
	Core::Maths::vec4 m_color{ 0.f,0.f,0.f,1.f };
	int m_keycode;
	const char* m_keyName;
	const char* VerifyKey(InputManager::KeyAction action);
};
