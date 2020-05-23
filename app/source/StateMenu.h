#pragma once
#include "State.h"
#include "Maths/vec4.h"
#include "GLFW/glfw3.h"
class StateMenu : public State
{
public:
	StateMenu(GLFWwindow* window);
	~StateMenu() = default;

	GameState GetState() const override { return GameState::MENU; }
	virtual GameState OnUpdate(float deltaTime) override;
	virtual void OnEnd() override;
	void OnBegin() override;

private:
	Core::Maths::vec4 m_color{ 0.f,0.f,0.f,1.f };
	bool m_settings = false;
};
