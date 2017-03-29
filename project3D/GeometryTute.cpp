#include "GeometryTute.h"

#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include <Gizmos.h>


using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Vertex
{
	vec4 position;
	vec4 colour;
};

GeometryTute::~GeometryTute()
{
}

void GeometryTute::GenerateGrid(unsigned int rows, unsigned int cols)
{
	//Creates an Array of Vertices to form a grid
	Vertex* aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
		for (unsigned int c = 0; c < cols; ++c)
		{
			aoVertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);

			//Create an arbitrary colour
			vec3 colour = vec3(sinf((c / (float)(cols - 1)) * (r / (float)(rows - 1))));
			
			aoVertices[r * cols + c].colour = vec4(colour, 1);
		}

	//Define an Index count based off Quad Count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];

	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			//Triangle 1
			auiIndices[index++] = r * cols + c; 
			auiIndices[index++] = (r + 1) * cols + c; 
			auiIndices[index++] = (r + 1) * cols + (c + 1);

			//Triangle 2
			auiIndices[index++] = r * cols + c; 
			auiIndices[index++] = (r + 1) * cols + (c + 1); 
			auiIndices[index++] = r * cols + (c + 1);
		}

	
	

	//Enable Vertex Attribute Arrays at specified Indexes (0 and 1)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	
	//Defines an Array of Vertex Attribute Data
	//Which Array, Size of each attribute, Type of Data, Normalized, Byte offset between Attribute Data
	//Offset of the first component of the first vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(vec4)));
	
	

	

	// ------ VERTEX AND INDECIES CODE ------
	//Creates and Binds buffers to a vertex Array Object
	glGenBuffers(1, &m_VBO);

	//Generate Index Buffer Objext
	glGenBuffers(1, &m_IBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Binds Buffer to a Vertex Array Buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//Binds it to Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	//Enters the Size of the Data as well as the data itself
	//Defines Useage:- Defined Once, used many times(Drawn)
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);
	//Enters Data and size of Data
	//Useage - Draw
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	

	delete[] auiIndices;
	delete[] aoVertices;
	
}

bool GeometryTute::Startup()
{
	//SetBackColour(0.25, 0.25, 0.25, 1);

	// ------ SHADER CODE ------
	//Create Shaders
	const char* vsSource = "#version 410\n \
							layout(location = 0) in vec4 position; \
							layout(location = 1) in vec4 colour; \
							out vec4 vColour; \
							uniform mat4 projectionViewWorldMatrix; \
							void main() { vColour = colour; gl_Position = projectionViewWorldMatrix * position; }";

	const char* fsSource = "#version 410\n \
							in vec4 vColour; \
							out vec4 fragColour; \
							void main() { fragColour = vColour; }";

	int success = GL_FALSE;
	//Creates Vertex and Fragment Shaders
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Passes in the Shader, count, source, length
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);

	// ^^^^^^
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		//Creates an array the length of the info log
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		//Assigns Data to InfoLog and prints it
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	GenerateGrid(m_rows, m_cols);

	return true;
}

void GeometryTute::Shutdown()
{
	


	aie::Gizmos::destroy();
}

void GeometryTute::Update(float deltaTime)
{

	if (glfwWindowShouldClose(m_window) == 0 && glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		Quit();
	}
}

void GeometryTute::Draw()
{
	ClearScreen();

	// ------ BINDING AND DRAWING ------
	glUseProgram(m_programID);
	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));

	//Binding Our Shader Program, instructing what shaders should be used for next draw call
	glBindVertexArray(m_VAO);
	unsigned int indexCount = (m_rows - 1) * (m_cols - 1) * 6;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	m_projectionViewMatrix = projection * view;

	aie::Gizmos::draw(m_projectionViewMatrix);

	
}