#include "hge-glfw-window.hpp"
#include <iostream>
#define HGETERMINATE std::cerr << "Termination in line: " << __LINE__ << " file: " << __FILE__ << std::endl; throw std::string("Termination.");
std::shared_ptr<hge::core::Application> hge::ui::HGEGLFWWindow::application;
double hge::ui::HGEGLFWWindow::lastCursorXposition = 0.0;
double hge::ui::HGEGLFWWindow::lastCursorYposition = 0.0;
void
hge::ui::HGEGLFWWindow::onErrorEvent
(int errorNumber,
		const char* errorDescription)
{
	std::cerr << "Error number is: "
			<< errorNumber
			<< std::endl
			<< "Error description: "
			<< errorDescription
			<< std::endl;
	HGETERMINATE
}
void
hge::ui::HGEGLFWWindow::onKeyEvent
(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	switch(action)
	{
		case(GLFW_PRESS):
			switch(key)
			{
				case(GLFW_KEY_ESCAPE):
					glfwSetWindowShouldClose(window, GL_TRUE);
					break;
				case(GLFW_KEY_F1):
					application->buttonPressed(core::Application::F1KeyButton);
					break;
				case(GLFW_KEY_W):
					application->buttonPressed(core::Application::wKeyButton);
					break;
				case(GLFW_KEY_A):
					application->buttonPressed(core::Application::aKeyButton);
					break;
				case(GLFW_KEY_S):
					application->buttonPressed(core::Application::sKeyButton);
					break;
				case(GLFW_KEY_D):
					application->buttonPressed(core::Application::dKeyButton);
					break;
				case(GLFW_KEY_UP):
					application->buttonPressed(core::Application::upArrowKeyButton);
					break;
				case(GLFW_KEY_DOWN):
					application->buttonPressed(core::Application::downArrowKeyButton);
					break;
				case(GLFW_KEY_LEFT):
					application->buttonPressed(core::Application::leftArrowKeyButton);
					break;
				case(GLFW_KEY_RIGHT):
					application->buttonPressed(core::Application::rightArrowKeyButton);
					break;
				case(GLFW_KEY_PAGE_UP):
					application->buttonPressed(core::Application::PageUpButton);
					break;
				case(GLFW_KEY_PAGE_DOWN):
					application->buttonPressed(core::Application::PageDownButton);
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
					application->buttonReleased(core::Application::wKeyButton);
					break;
				case(GLFW_KEY_A):
					application->buttonReleased(core::Application::aKeyButton);
					break;
				case(GLFW_KEY_S):
					application->buttonReleased(core::Application::sKeyButton);
					break;
				case(GLFW_KEY_D):
					application->buttonReleased(core::Application::dKeyButton);
					break;
				case(GLFW_KEY_UP):
					application->buttonReleased(core::Application::upArrowKeyButton);
					break;
				case(GLFW_KEY_DOWN):
					application->buttonReleased(core::Application::downArrowKeyButton);
					break;
				case(GLFW_KEY_LEFT):
					application->buttonReleased(core::Application::leftArrowKeyButton);
					break;
				case(GLFW_KEY_RIGHT):
					application->buttonReleased(core::Application::rightArrowKeyButton);
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
void hge::ui::HGEGLFWWindow::onMouseKeyEvent(GLFWwindow* window, int button, int action, int mods)
{
	switch(action)
	{
		case GLFW_PRESS:
			switch(button)
			{
				case GLFW_MOUSE_BUTTON_LEFT:
					application->buttonPressed(core::Application::leftMouseButton);
					break;
				case GLFW_MOUSE_BUTTON_RIGHT:
					application->buttonPressed(core::Application::rightMouseButton);
					break;
				case GLFW_MOUSE_BUTTON_MIDDLE:
					application->buttonPressed(core::Application::middleMouseButton);
					break;
				default:
					break;
			}
			break;
		case GLFW_RELEASE:
			switch(button)
			{
				case GLFW_MOUSE_BUTTON_LEFT:
					application->buttonReleased(core::Application::leftMouseButton);
					break;
				case GLFW_MOUSE_BUTTON_RIGHT:
					application->buttonReleased(core::Application::rightMouseButton);
					break;
				case GLFW_MOUSE_BUTTON_MIDDLE:
					application->buttonReleased(core::Application::middleMouseButton);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}
void
hge::ui::HGEGLFWWindow::onCursorMoveEvent(GLFWwindow* window, double x, double y)
{
	application->mouseMoved((float)(x - lastCursorXposition),(float)(y - lastCursorYposition));
	lastCursorXposition = x;
	lastCursorYposition = y;
}
void
hge::ui::HGEGLFWWindow::onChangeSizeEvent
(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void
hge::ui::HGEGLFWWindow::start
(const std::shared_ptr<hge::core::Application>& app)
{
	application = app;
	application->start();
	GLFWwindow* window;
	glfwSetErrorCallback(onErrorEvent);
	if(!glfwInit())
	{
		HGETERMINATE
	}
	auto priMon = glfwGetPrimaryMonitor();
	auto vidMod = glfwGetVideoMode(priMon);
	window = glfwCreateWindow(vidMod->width, vidMod->height, "Hulixerian Game Engine", priMon, NULL);
	if (!window)
	{
		glfwTerminate();
		HGETERMINATE
	}
	glfwMakeContextCurrent(window);
	if(glewInit() != GLEW_OK)
	{
		HGETERMINATE
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
}
void
hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate()
{
	switch(glGetError())
	{
		case GL_NO_ERROR :
			break;
		case GL_INVALID_ENUM :
			std::cout << "Invalid enum error." << std::endl;
			std::terminate();
			break;
		case GL_INVALID_VALUE :
			std::cout << "Invalid value error." << std::endl;
			std::terminate();
			break;
		case GL_INVALID_INDEX :
			std::cout << "Invalid index error." << std::endl;
			std::terminate();
			break;
		case GL_INVALID_OPERATION :
			std::cout << "Invalid operation error." << std::endl;
			std::terminate();
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION :
			std::cout << "Invalid framebuffer operation error." << std::endl;
			std::terminate();
			break;
		case GL_OUT_OF_MEMORY :
			std::cout << "Out of memory error." << std::endl;
			std::terminate();
			break;
		case GL_STACK_OVERFLOW :
			std::cout << "Stack overflow error." << std::endl;
			std::terminate();
			break;
		case GL_STACK_UNDERFLOW :
			std::cout << "Stack underflow error." << std::endl;
			std::terminate();
			break;
		default :
			std::cout << "Unknown error." << std::endl;
			std::terminate();
			break;
	}
}
