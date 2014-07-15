#include "hge-director.hpp"
#include "hge-tcp-connector.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
static hge::core::ApplicationUnit *application;
static double lastCursorXposition;
static double lastCursorYposition;
static void onErrorEvent(const int errorNumber, const char *errorDescription)
{
	std::cerr << "Error number is: "
			<< errorNumber
			<< std::endl
			<< "Error description: "
			<< errorDescription
			<< std::endl;
}
static void onKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	(void) scanCode;
	(void) mods;
	switch(action)
	{
		case(GLFW_PRESS):
			switch(key)
			{
				case(GLFW_KEY_ESCAPE):
					glfwSetWindowShouldClose(window, GL_TRUE);
					break;
				case(GLFW_KEY_F1):
					application->buttonPressed(hge::core::ApplicationUnit::F1KeyButton);
					break;
				case(GLFW_KEY_W):
					application->buttonPressed(hge::core::ApplicationUnit::wKeyButton);
					break;
				case(GLFW_KEY_A):
					application->buttonPressed(hge::core::ApplicationUnit::aKeyButton);
					break;
				case(GLFW_KEY_S):
					application->buttonPressed(hge::core::ApplicationUnit::sKeyButton);
					break;
				case(GLFW_KEY_D):
					application->buttonPressed(hge::core::ApplicationUnit::dKeyButton);
					break;
				case(GLFW_KEY_UP):
					application->buttonPressed(hge::core::ApplicationUnit::upArrowKeyButton);
					break;
				case(GLFW_KEY_DOWN):
					application->buttonPressed(hge::core::ApplicationUnit::downArrowKeyButton);
					break;
				case(GLFW_KEY_LEFT):
					application->buttonPressed(hge::core::ApplicationUnit::leftArrowKeyButton);
					break;
				case(GLFW_KEY_RIGHT):
					application->buttonPressed(hge::core::ApplicationUnit::rightArrowKeyButton);
					break;
				case(GLFW_KEY_PAGE_UP):
					application->buttonPressed(hge::core::ApplicationUnit::PageUpButton);
					break;
				case(GLFW_KEY_PAGE_DOWN):
					application->buttonPressed(hge::core::ApplicationUnit::PageDownButton);
					break;
				default:
					break;
			}
			break;
		case(GLFW_RELEASE):
			switch(key)
			{
				case(GLFW_KEY_ESCAPE):
					break;
				case(GLFW_KEY_W):
					application->buttonReleased(hge::core::ApplicationUnit::wKeyButton);
					break;
				case(GLFW_KEY_A):
					application->buttonReleased(hge::core::ApplicationUnit::aKeyButton);
					break;
				case(GLFW_KEY_S):
					application->buttonReleased(hge::core::ApplicationUnit::sKeyButton);
					break;
				case(GLFW_KEY_D):
					application->buttonReleased(hge::core::ApplicationUnit::dKeyButton);
					break;
				case(GLFW_KEY_UP):
					application->buttonReleased(hge::core::ApplicationUnit::upArrowKeyButton);
					break;
				case(GLFW_KEY_DOWN):
					application->buttonReleased(hge::core::ApplicationUnit::downArrowKeyButton);
					break;
				case(GLFW_KEY_LEFT):
					application->buttonReleased(hge::core::ApplicationUnit::leftArrowKeyButton);
					break;
				case(GLFW_KEY_RIGHT):
					application->buttonReleased(hge::core::ApplicationUnit::rightArrowKeyButton);
					break;
				default:
					break;
			}
			break;
		case(GLFW_REPEAT):
			break;
		default:
			break;
	}
}
static void onMouseKeyEvent(GLFWwindow* window, int button, int action, int mods)
{
	(void) window;
	(void) mods;
	switch(action)
	{
		case GLFW_PRESS:
			switch(button)
			{
				case GLFW_MOUSE_BUTTON_LEFT:
					application->buttonPressed(hge::core::ApplicationUnit::leftMouseButton);
					break;
				case GLFW_MOUSE_BUTTON_RIGHT:
					application->buttonPressed(hge::core::ApplicationUnit::rightMouseButton);
					break;
				case GLFW_MOUSE_BUTTON_MIDDLE:
					application->buttonPressed(hge::core::ApplicationUnit::middleMouseButton);
					break;
				default:
					break;
			}
			break;
		case GLFW_RELEASE:
			switch(button)
			{
				case GLFW_MOUSE_BUTTON_LEFT:
					application->buttonReleased(hge::core::ApplicationUnit::leftMouseButton);
					break;
				case GLFW_MOUSE_BUTTON_RIGHT:
					application->buttonReleased(hge::core::ApplicationUnit::rightMouseButton);
					break;
				case GLFW_MOUSE_BUTTON_MIDDLE:
					application->buttonReleased(hge::core::ApplicationUnit::middleMouseButton);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
static void onCursorMoveEvent(GLFWwindow* window, double x, double y)
{
	(void) window;
	application->mouseMoved((float)(x - lastCursorXposition),(float)(y - lastCursorYposition));
	lastCursorXposition = x;
	lastCursorYposition = y;
}
static void onChangeSizeEvent(GLFWwindow* window, int width, int height)
{
	(void) window;
	glViewport(0, 0, width, height);
}
int main(int argc, char ** argv)
{
	(void) argc;
	(void) argv;
	hge::utility::TCPConnector *connector = new hge::utility::TCPConnector();
	application = new hge::core::Director();
	connector->setApplication(application);
	connector->startConnection();
	application->start();
	GLFWwindow* window;
	glfwSetErrorCallback(onErrorEvent);
	if(!glfwInit())
	{
		std::cerr << "Error in initializing GLFW3, in file:" << __FILE__ << " in line:" << __LINE__;
		return 1;
	}
	auto priMon = glfwGetPrimaryMonitor();
	auto vidMod = glfwGetVideoMode(priMon);
	window = glfwCreateWindow(800, 600 /*vidMod->width, vidMod->height*/, "Hulixerian Game Engine", 0/*priMon*/, NULL);
	if(!window)
	{
		glfwTerminate();
		std::cerr << "Error in opening GLFW3 window, in file:" << __FILE__ << " in line:" << __LINE__;
		return 1;
	}
	glfwMakeContextCurrent(window);
	if(glewInit() != GLEW_OK)
	{
		std::cerr << "Error in initializing GLEW in file:" << __FILE__ << " in line:" << __LINE__;
		return 1;
	}
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_ALPHA_TEST);
	glfwSetKeyCallback(window, onKeyEvent);
	glfwSetMouseButtonCallback(window, onMouseKeyEvent);
	glfwSetCursorPosCallback(window, onCursorMoveEvent);
	glViewport(0, 0, vidMod->width, vidMod->height);
	application->initialize();
	glfwSetFramebufferSizeCallback(window, onChangeSizeEvent);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		application->update();
		glFinish();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
