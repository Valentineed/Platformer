#include "FSM.h"
#include "StatePlay.h"
#include "StateMenu.h"
#include "StateOption.h"
#include "StatePause.h"
#include "StateEdit.h"

FSM::FSM(GLFWwindow* window)
{
	m_state[(int)GameState::MENU] = new StateMenu(window);
	m_state[(int)GameState::PLAY] = new StatePlay();
	m_state[(int)GameState::SETTINGS] = new StateOption();
	m_state[(int)GameState::PAUSE] = new StatePause();
	m_state[(int)GameState::EDIT] = new StateEdit();

	m_currentState = m_state[(int)GameState::MENU];
}

FSM::~FSM()
{
	for (size_t i = 0; i < (int)GameState::NO_STATE; i++)
	{
		delete m_state[i];
	}
}

GameState FSM::GetState() const
{
	return m_currentState->GetState();
}

void FSM::ChangeState(GameState state)
{
	m_currentState->OnEnd();
	m_currentState = m_state[(int)state];
	m_currentState->OnBegin();
}

void FSM::Update(float deltaTime)
{
	GameState next = m_currentState->OnUpdate(deltaTime);
	if (next != m_currentState->GetState())
	{
		ChangeState(next);
	}
}