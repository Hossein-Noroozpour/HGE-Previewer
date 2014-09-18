#include "hge-director.hpp"
#include "hge-terrain-unit.hpp"
#include "hge-terrain-sun-shader.hpp"
#include "hge-protocol.hpp"
#include "hge-skybox-shader-unit.hpp"
#include "hge-resource-manager.hpp"
#include <thread>
#include <iostream>
#include <fstream>
#define HGEPRINTCODELINE std::cout << "Debugging: file:" << __FILE__ << " line:" << __LINE__ << std::endl << std::flush;
hge::core::Director::Director():
	goForward(false),
	goDownward(false),
	goRightward(false),
	goLeftward(false),
	rotateOn(false),
	wireframeMode(false),
	cameraRotationSpeed(0.001f),
	cameraMoveSpeed(5.0f)
{
}
hge::core::Director::~Director()
{
}
void hge::core::Director::start()
{
}
void hge::core::Director::initialize()
{
	/// Testing part
	std::string logoFileAddress("C:\\Users\\Thany Hulixer\\Documents\\Visual Studio 2013"
		"\\Projects\\HGE-Previewer\\x64\\Debug\\HGE-Logo1024x1024.png");
	std::ifstream logoFileStream;
	logoFileStream.open(logoFileAddress, std::ios_base::binary);
	if (logoFileStream.is_open())
	{
		std::shared_ptr<hge::texture::TextureUnit> logoTexture(new hge::texture::TextureUnit(GL_TEXTURE_2D, logoFileStream));
		render::SceneUnit::defaultTexture = logoTexture;
	}
	else
	{
		std::cerr << "Error in logo file opening process." << std::endl;
		std::terminate();
	}
	render::SceneUnit::defaultShader = std::shared_ptr<shader::SunShader>(new shader::SunShader());
	/////////////////////////////////////////////////////////////////
	scene = ResourceManager::importScene("C:\\Users\\Thany Hulixer\\Documents\\Projects\\HGE\\HGE-Builder\\hge-sample.hge");
}
void hge::core::Director::update()
{
	gldo_lock.lock();
	for(auto f : gldo)
	{
		f();
	}
	gldo.clear();
	gldo_lock.unlock();
	if(goForward)
	{
		scene->getCamera()->moveForward(cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	if(goDownward)
	{
		scene->getCamera()->moveForward(-cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	if(goRightward)
	{
		scene->getCamera()->moveSideward(cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	if(goLeftward)
	{
		scene->getCamera()->moveSideward(-cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	scene->draw();
	////////////////////////////////////////////////////////
}
void hge::core::Director::buttonPressed(const HGEButton& key)
{
	switch(key)
	{
		case(F1KeyButton):
			if(wireframeMode)
			{
				wireframeMode = false;
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				wireframeMode = true;
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;
		case(PageUpButton):
			cameraMoveSpeed *= 2;
			break;
		case(PageDownButton):
			cameraMoveSpeed /= 2;
			break;
		case(wKeyButton):
			goForward = true;
			break;
		case(sKeyButton):
			goDownward = true;
			break;
		case(aKeyButton):
			goLeftward = true;
			break;
		case(dKeyButton):
			goRightward = true;
			break;
		case(middleMouseButton):
			rotateOn = true;
			break;
		default:
			break;
	}
}
void hge::core::Director::buttonReleased(const HGEButton& key)
{
	switch(key)
	{
		case(wKeyButton):
			goForward = false;
			break;
		case(sKeyButton):
			goDownward = false;
			break;
		case(aKeyButton):
			goLeftward = false;
			break;
		case(dKeyButton):
			goRightward = false;
			break;
		case(middleMouseButton):
			rotateOn = false;
			break;
		default:
			break;
	}
}
void hge::core::Director::mouseMoved(const float &dx, const float &dy)
{
	if(rotateOn)
	{
		scene->getCamera()->rotateGlobalZ(dx * cameraRotationSpeed);
		scene->getCamera()->rotateLocalX(dy * cameraRotationSpeed);
	}
}
