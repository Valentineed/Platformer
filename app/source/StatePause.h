#pragma once
#include "State.h"

class StatePause : public State
{
public:
	StatePause() = default;
	~StatePause() = default;

	GameState GetState() const override { return GameState::PAUSE; }
	virtual GameState OnUpdate(float deltaTime) override;
	virtual void OnEnd() override;
	void OnBegin() override;
private:
	static bool m_popWindowSave;
	void PopWindow();

};
