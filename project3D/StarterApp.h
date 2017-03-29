#pragma once
#include "MyApplicationClass.h"

#include <glm/mat4x4.hpp>

class StarterApp : public MyApplicationClass
{
public:
	StarterApp();
	~StarterApp();

	bool Startup();
	void Shutdown();

	void Update(float a_deltaTime);
	void Draw();

private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

};

