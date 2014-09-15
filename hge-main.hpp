/**
 * Author: Hossein Noroozpour Thany Abady
 * Email: Hossein.Noroozpour@gmail.com
 */
#ifndef HGE_MAIN_HPP
#define HGE_MAIN_HPP
#include "hge-application-unit.hpp"
#include <memory>
#include "hge-camera-unit.hpp"
#include "hge-perspective-unit.hpp"
#include "hge-scene-unit.hpp"
#include "hge-sun-shader.hpp"
#include "hge-texture-unit.hpp"
#include "hge-terrain-sun-shader.hpp"
class MyGame : public hge::core::ApplicationUnit
{
public:
	MyGame();
	virtual ~MyGame();
	void start();
	void initialize();
	void update();
	void buttonPressed(const HGEButton& key);
	void buttonReleased(const HGEButton& key);
	void mouseMoved(const float& dx, const float& dy);
private:
	std::shared_ptr<hge::render::SceneUnit> scene;
	std::shared_ptr<hge::render::TerrainUnit> terrain;
	std::shared_ptr<hge::shader::TerrainSunShader> terrainShader;
	bool goForward;
	bool goDownward;
	bool goRightward;
	bool goLeftward;
	bool wireframeMode;
	float m_cameraRotationSpeed;
	float m_cameraMoveSpeed;
	void swapBytes(unsigned char *const& bytes, const unsigned int &size);
};
#ifdef ANDROID
#include <jni.h>
#ifdef __cplusplus
extern "C"
{
#endif
	JNIEXPORT jboolean JNICALL Java_com_hulixerian_hge_HGELib_init (JNIEnv *, jclass);
	JNIEXPORT jboolean JNICALL Java_com_hulixerian_hge_HGELib_resize (JNIEnv *, jclass, jint, jint);
	JNIEXPORT jboolean JNICALL Java_com_hulixerian_hge_HGELib_render (JNIEnv *, jclass);
#ifdef __cplusplus
}
#endif
#endif
#endif
