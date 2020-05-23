#include "App.h"
#include <iostream>
#include "ResourceGFX.h" 
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "PhysicsWorld.h"
#include "DebugDrawer.h"
#include "Resources/Scene.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_iconify_callback(GLFWwindow* window, int iconified);
void window_maximize_callback(GLFWwindow* window, int maximized);
App* App::m_instance = nullptr;

App* App::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new App();
		return m_instance;
	}
	return m_instance;
}

App::App()
{
	Init();
}

void App::Init()
{
	//--- GLFW ---//
	InitGLFW();
	ConfigGLFW();

	//--- CALLBACKS ---//
	SetWindowCallbacks();

	//--- LIBGFX ---//
	InitLibrary();

	// ------- Init Game ------//
	m_debugDrawer = new DebugDrawer();
	PhysicsWorld::GetInstance()->SetDebugDrawer(m_debugDrawer);
	PhysicsWorld::GetInstance()->GetDynamicsWorld()->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_inputManager = new InputManager(m_window);
	m_fsm = new FSM(m_window);
	m_scene.InitScene();
}

App::~App()
{
	delete m_inputManager;
	delete m_fsm;
	delete m_debugDrawer;
}

void App::InitGLFW()
{
	/* Initialize the library */
	if (!glfwInit())
		return exit(-1);

	//-------------- glfw window creation / option genrete fullscreen ---------------//
	//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MTL_ModernOpenGL", glfwGetPrimaryMonitor(), nullptr); 
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "MTL_ModernOpenGL", nullptr, nullptr);
	if (!m_window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return exit(-1);
	}

	glfwMakeContextCurrent(m_window);
	
}

void App::ConfigGLFW()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//---> Set a version OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);//-/
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	glfwWindowHint(GLFW_DECORATED, true);
	glfwWindowHint(GLFW_ACCUM_RED_BITS, 8);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwSetGamma(glfwGetPrimaryMonitor(), 1.);//NICEEEEEEE

	//Set icon Window
	SetIconWindow("resource/downhillduck.jpg");
}

void App::SetWindowCallbacks()
{
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetWindowIconifyCallback(m_window, window_iconify_callback);
	glfwSetWindowMaximizeCallback(m_window, window_maximize_callback);
}

void App::InitLibrary()
{
	// glad: load all OpenGL function pointers
	if (!InitLibGraph((GraphicsInitFunction)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return exit(-1);
	}
}

void App::SetDeltaTime(float time)
{
	m_delta = time - m_lastTime;
	m_lastTime = time;
}

void App::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		//--- Set deltaTime ---//
		SetDeltaTime((float)glfwGetTime());
		
		m_fsm->Update(m_delta);

		/* Swap front and back buffers */
		glfwSwapBuffers(m_window);

		/*Poll for and process events*/
		m_inputManager->ProcessInput(m_window, m_delta);
		glfwPollEvents();
	}
	/*Clean ImGUI*/
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	_CrtDumpMemoryLeaks();
	return exit(0);
}

DebugDrawer* App::GetDebugDrawer() const
{
	return m_debugDrawer;
}

InputManager* App::GetInputManager() const
{
	return m_inputManager;
}

FSM* App::GetFSM() const
{
	return m_fsm;
}


void App::SetIconWindow(const char* path)
{
	{
		GLFWimage icons;
		Resources::TemporaryTexture test = Resources::TemporaryTexture::TemporaryTexture(path, &icons.width, &icons.height, 0, 4);
		icons.pixels = test.m_pixels;
		if (icons.pixels)
		{
			glfwSetWindowIcon(m_window, 1, &icons);
		}
		else
		{
			std::cerr << "Failed to load Window Icon" << std::endl;
		}
	}
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
	App::GetInstance()->m_windowWidth = width;
	App::GetInstance()->m_windowHeight = height;
	// Size of Window for the rendering
	SetViewPort(0, 0, width, height);
}

void window_iconify_callback(GLFWwindow* window, int iconified)
{
	if (iconified)
	{
		glfwIconifyWindow(window);
	}
	else
	{
		glfwRestoreWindow(window);
	}
}

void window_maximize_callback(GLFWwindow* window, int maximized)
{
	if (maximized)
	{
		glfwMaximizeWindow(window);
	}
	else
	{
		glfwRestoreWindow(window);
	}
}




//------------- Curiosity --------------//
/* Interesting
	int xpos, ypos, workWidth, workHeight;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &xpos, &ypos, &workWidth, &workHeight);
*/