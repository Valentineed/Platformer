#pragma once
#include "State.h"
#include "GLFW/glfw3.h"
class FSM
{
public:
	FSM(GLFWwindow* window);
	~FSM();

	GameState GetState()const;
	void ChangeState(GameState state);
	void Update(float deltaTime);

private:
	State* m_state[(int)GameState::NO_STATE];
	State* m_currentState = nullptr;
};