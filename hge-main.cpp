/**
 * Author: Hossein Noroozpour Thany Abady
 * Email: Hossein.Noroozpour@gmail.com
 */
#include "hge-main.hpp"
#include <fstream>
#include <iostream>
#ifdef ANDROID
#ifdef __cplusplus
extern "C"
{
#endif // End of __cplusplus
	JNIEXPORT jboolean JNICALL Java_com_hulixerian_hge_HGELib_init (JNIEnv *pEnv, jclass clazz)
	{
		return 1;
	}
	JNIEXPORT jboolean JNICALL Java_com_hulixerian_hge_HGELib_resize (JNIEnv *pEnv, jclass clazz, jint width, jint height)
	{
		return 1;
	}
	JNIEXPORT jboolean JNICALL Java_com_hulixerian_hge_HGELib_render (JNIEnv *pEnv, jclass clazz)
	{
		return 1;
	}
#ifdef __cplusplus
}
#endif // End of __cplusplus
#else // End of ANDROID
#include "hge-glfw-window.hpp"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	std::shared_ptr<hge::core::ApplicationUnit> app(new MyGame());
	hge::ui::HGEGLFWWindow::start(app);
	return 0;
}
#endif
MyGame::MyGame()
{}
MyGame::~MyGame()
{}
void MyGame::start()
{}
void MyGame::initialize()
{
	std::string logoFileAddress("C:\\Users\\Thany Hulixer\\Documents\\Visual Studio 2013\\Projects\\HGE-Previewer\\x64\\Debug\\HGE-Logo1024x1024.png");
	std::ifstream logoFileStream;
	logoFileStream.open(logoFileAddress, std::ios_base::binary);
	if (logoFileStream.is_open())
	{
		std::shared_ptr<hge::texture::TextureUnit> logoTexture(new hge::texture::TextureUnit(GL_TEXTURE_2D, logoFileStream));
	}
	else
	{
		std::cerr << "Error in logo file opening process." << std::endl;
		std::terminate();
	}
}
void MyGame::update()
{}
void MyGame::buttonPressed(const HGEButton& key)
{}
void MyGame::buttonReleased(const HGEButton& key)
{}
void MyGame::mouseMoved(const float& dx, const float& dy)
{}
