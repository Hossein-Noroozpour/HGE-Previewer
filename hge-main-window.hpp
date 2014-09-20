#ifndef HGE_MAIN_WINDOW_HPP
#define	HGE_MAIN_WINDOW_HPP
#include "hge-configure.hpp"
#ifdef HGE_USE_OPENGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#else
#include <windows.h>
#endif
#include "hge-application-unit.hpp"
#include <memory>
namespace hge
{
	namespace ui
	{
		class HGEGLFWWindow
		{
		public:
			static void start(const std::shared_ptr<core::ApplicationUnit>& application);
		private:
#ifdef HGE_USE_OPENGL
			static void onErrorEvent(int errorNumber, const char* errorDescription);
			static void onCursorMoveEvent(GLFWwindow* window, double x, double y);
			static void onMouseKeyEvent(GLFWwindow* window, int button, int action, int mods);
			static void onKeyEvent(GLFWwindow* window, int key, int scanCode, int action, int mods);
			static void onChangeSizeEvent(GLFWwindow* window, int width, int height);
			static void printOpenGLErrorAndTerminate();
#else
			static HINSTANCE hge_hInst;
			static HWND hge_hWnd;
#endif
			static std::shared_ptr<core::ApplicationUnit> application;
			static double lastCursorXposition;
			static double lastCursorYposition;
		};
	}
}
#endif
