#pragma once

enum class GameState
{
	MENU,
	PLAY,
	SETTINGS,
	PAUSE,
	EDIT,

	NO_STATE //keep this entry last
};

class State
{
public:

	State() = default;
	virtual ~State() = default;

	virtual GameState GetState() const = 0;
	virtual void OnBegin() = 0;
	virtual GameState OnUpdate(float deltaTime) = 0;
	virtual void OnEnd() = 0;

private:

};