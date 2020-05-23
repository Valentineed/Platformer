// MTL_ModernOpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Resources/Scene.h"
#include "App.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <sstream>
#include <string>

// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

int main()
{
	App::GetInstance()->Run();
}


