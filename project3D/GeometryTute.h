#pragma once
#include "MyApplicationClass.h"

#include <glm\glm.hpp>
#include <glm/ext.hpp>

class GeometryTute :
	public MyApplicationClass
{
public:
	GeometryTute(unsigned int rows, unsigned int cols) { m_rows = rows; m_cols = cols; }
	~GeometryTute();

	bool Startup();
	void Shutdown();

	void Update(float deltaTime);
	void Draw();

	void GenerateGrid(unsigned int rows, unsigned int cols);

private:
	//Vertex and Index Buffers
	unsigned int m_VAO;	//Vertex Attributes Object
	unsigned int m_VBO;	//Vertex Buffer Object
	unsigned int m_IBO;	//Index Buffer Object

	unsigned int m_programID;

	unsigned int m_rows;
	unsigned int m_cols;

	glm::mat4 m_projectionViewMatrix;

};

