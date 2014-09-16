#ifndef HGEGLFWWINDOW_HPP
#define	HGEGLFWWINDOW_HPP
#include <GL/glew.h>
#include "hge-application-unit.hpp"
#include <GLFW/glfw3.h>
#include <memory>
namespace hge
{
	namespace ui
	{
		class HGEGLFWWindow
		{
		public:
			static void start(const std::shared_ptr<core::ApplicationUnit>& application);
			static void onErrorEvent(int errorNumber, const char* errorDescription);
			static void onCursorMoveEvent(GLFWwindow* window, double x, double y);
			static void onMouseKeyEvent(GLFWwindow* window, int button, int action, int mods);
			static void onKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods);
			static void onChangeSizeEvent(GLFWwindow* window, int width, int height);
			static void printOpenGLErrorAndTerminate();
		private:
			static std::shared_ptr<core::ApplicationUnit> application;
			static double lastCursorXposition;
			static double lastCursorYposition;
		};
	}
}
#endif
