#pragma once
#include "State.h"
#include "App.h"

class StatePlay : public State
{

public:
	StatePlay();
	~StatePlay() = default;

	GameState GetState() const override { return GameState::PLAY; }
	virtual GameState OnUpdate(float deltaTime) override;
	virtual void OnEnd() override;
	void OnBegin() override;
	void DisplayLifePlayer();
	//void MoveCamera(LowRenderer::MovCamera& dir);

	
private:
	
	float m_delta = 0.f;
	GameState m_currentState = GameState::PLAY;

};