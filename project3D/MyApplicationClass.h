#pragma once

//Forward declaration struct for access to GLFW Window
struct GLFWwindow;

class MyApplicationClass
{
public:

	MyApplicationClass();
	virtual ~MyApplicationClass();

	//Creates a window and starts the Game Loop, which calls update and draw
	//Calls Startup and if successful begins the loop
	//Ends with Shutdown if m_gameOver = true
	void Run(const char* a_title, int a_width, int a_height, bool a_fullscreen);

	//Self Explanatory
	virtual bool Startup() = 0;
	virtual void Shutdown() = 0;

	virtual void Update(float deltaTime) = 0;
	virtual void Draw() = 0;

	//Clears screen to begin drawing
	void ClearScreen();

	//Sets the Background colour the screen is cleared to
	void SetBackColour(float a_r, float a_g, float a_b, float a_a = 1.0f);

	//Sets m_gameOver to true which safely closes the application
	void Quit() { m_gameOver = true; }

	//Returns access to the GLFW Window
	GLFWwindow* GetWindowPtr() const { return m_window; }

	//Queries to check if Window has closed somehow
	bool HasWindowClosed();

	//Returns the Width/Height of the Game Window
	unsigned int GetWindowHeight() const;
	unsigned int GetWindowWidth() const;

	float GetTime();


protected:

	virtual bool CreateGameWindow(const char* a_title, int a_width, int a_height, bool a_fullscreen);
	virtual void DestroyWindow();

	GLFWwindow* m_window;

	bool m_gameOver;
};



