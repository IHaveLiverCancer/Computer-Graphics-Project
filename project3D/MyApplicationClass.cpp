#include "MyApplicationClass.h"


#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

MyApplicationClass::MyApplicationClass()
	: m_window(nullptr),
	m_gameOver(false) {
}



MyApplicationClass::~MyApplicationClass()
{
}

bool MyApplicationClass::CreateGameWindow(const char* a_title, int a_width, int a_height, bool a_fullscreen)
{
	//If glfw fails to initialise, return false
	if (glfwInit() == GL_FALSE)
		return false;

	//Sets m_window
	m_window = glfwCreateWindow(a_width, a_height, a_title, (a_fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	if (m_window == nullptr) //If it failed to initialise, return false
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	//If the OGL functions do not load, destroy GLFW window and terminate process, return false
	if(ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h) { glViewport(0, 0, w, h); });

	//Clear screen with Colour
	glClearColor(0, 0, 0, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
	return true;	

}

void MyApplicationClass::DestroyWindow()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void MyApplicationClass::Run(const char* a_title, int a_width, int a_height, bool a_fullscreen)
{
	if (CreateGameWindow(a_title, a_width, a_height, a_fullscreen) && Startup())
	{
		double prevTime = glfwGetTime();
		double currTime = 0;
		double deltaTime = 0;

		while (!m_gameOver)
		{
			//Update DeltaTime
			currTime = glfwGetTime();
			deltaTime = currTime - prevTime;
			prevTime = currTime;

			//Update window events
			glfwPollEvents();

			if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0)
				continue;

			Update(float(deltaTime));
			Draw();
			
			glfwSwapBuffers(m_window);

			m_gameOver = m_gameOver || glfwWindowShouldClose(m_window) == GLFW_TRUE;

		}
	}

	Shutdown();
	DestroyWindow();
}

bool MyApplicationClass::HasWindowClosed()
{
	return glfwWindowShouldClose(m_window) == GL_TRUE;
}

void MyApplicationClass::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void MyApplicationClass::SetBackColour(float a_r, float a_g, float a_b, float a_a)
{
	glClearColor(a_r, a_g, a_b, a_a);
}

unsigned int MyApplicationClass::GetWindowWidth() const
{
	int w = 0, h = 0;
	glfwGetWindowSize(m_window, &w, &h);
	return w;
}

unsigned int MyApplicationClass::GetWindowHeight() const
{
	int w = 0, h = 0;
	glfwGetWindowSize(m_window, &w, &h);
	return h;
}

float MyApplicationClass::GetTime()
{
	return (float)glfwGetTime();
}
