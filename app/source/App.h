#pragma once
#include <GLFW/glfw3.h>
#include <string>

#include "Resources/Scene.h"
#include "Resources/Texture.h"
#include "DebugDrawer.h"
#include "InputManager.h"
#include "FSM.h"
#include "Save.h"
#include "Player.h"


class App
{
public:
	static App* GetInstance();

	void Run();

	GLFWwindow* m_window = nullptr;

	DebugDrawer* GetDebugDrawer() const;

	InputManager* GetInputManager() const;

	FSM* GetFSM() const;

	float GetTime() const { return m_delta; }
	int m_windowWidth = 1250;
	int m_windowHeight = 600;



	//camera settings
	float m_lastX = (float)1250 / 2.f;
	float m_lastY = (float)600 / 2.f;
	bool m_active_mouse = true;
	bool m_mouse_cursor = true;
	bool m_firstMouse = true;
	bool m_wireframe = false;

	bool m_gameInit = false;

	Save m_save;
	Resources::Scene m_scene;

private:
	void Init();
	
	void InitGLFW();
	void InitLibrary();

	void ConfigGLFW();
	void SetWindowCallbacks();
	void SetIconWindow(const char*  path);

	//--- Time Settingts ---//
	void SetDeltaTime(float time);
	float m_lastTime = 0.f;
	float m_delta = 0.f;

	DebugDrawer* m_debugDrawer = nullptr;

	InputManager* m_inputManager = nullptr;
	FSM* m_fsm = nullptr;

	/*Singleton*/
	App();
	~App();
	static App* m_instance;
};