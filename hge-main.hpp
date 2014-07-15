#ifndef HGEMAIN_HPP
#define	HGEMAIN_HPP
#include "hge-application.hpp"
#include <memory>
#include "hge-camera-unit.hpp"
#include "hge-perspective-unit.hpp"
#include "hge-scene-unit.hpp"
#include "hge-sun-shader.hpp"
#include "hge-texture-unit.hpp"
#include "hge-terrain-sun-shader.hpp"
class MyGame : public hge::core::Application
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
#endif
