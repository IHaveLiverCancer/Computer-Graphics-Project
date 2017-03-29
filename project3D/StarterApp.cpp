#include "StarterApp.h"

#include <Gizmos.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;


StarterApp::StarterApp()
{
}


StarterApp::~StarterApp()
{
}

bool StarterApp::Startup()
{
	SetBackColour(0.25f, 0.25, 0.25f, 1.0f);

	//Initlialize the Gizmos count

	Gizmos::create(10000, 10000, 10000, 10000);



	//Create the Camera Transforms
	m_viewMatrix = glm::lookAt(vec3(20), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		GetWindowWidth() / (float)GetWindowHeight(),
		0.1f, 1000.f);


	return true;
}

void StarterApp::Shutdown()
{
	Gizmos::destroy();
}

void StarterApp::Update(float a_deltaTime)
{
	float time = GetTime();

	//Gently rotate the camera
	//m_viewMatrix = glm::lookAt(vec3(glm::sin(time) * 10, 10, glm::cos(time) * 10), vec3(0), vec3(0, 1, 0));
								

	Gizmos::clear();

	//Draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	//If Escape is pressed, close the window
	if (glfwWindowShouldClose(m_window) == 0 && glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		Quit();
	}
}

void StarterApp::Draw()
{
	//Wipe the screen to Back Colour
	ClearScreen();

	//Update perspective if window is resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		GetWindowWidth() / (float)GetWindowHeight(),
		0.1f, 1000.f);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}