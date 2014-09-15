#include "hge-director.hpp"
#include "hge-terrain-unit.hpp"
#include "hge-terrain-sun-shader.hpp"
#include "hge-protocol.hpp"
#include "hge-skybox-shader-unit.hpp"
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
	m_cameraRotationSpeed(0.001f),
	m_cameraMoveSpeed(5.0f)
{
}
hge::core::Director::~Director()
{
}
void hge::core::Director::start()
{
}
static void swapBytes(unsigned char * const &bytes, const unsigned int &size)
{
	unsigned char tmpuc;
	for(unsigned int i = 0, j = size - 1; i < j; i++, j--)
	{
		tmpuc = bytes[i];
		bytes[i] = bytes[j];
		bytes[j] = tmpuc;
	}
}
void hge::core::Director::initialize()
{
	
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
		scene->getCamera()->moveForward(m_cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	if(goDownward)
	{
		scene->getCamera()->moveForward(-m_cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	if(goRightward)
	{
		scene->getCamera()->moveSideward(m_cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	if(goLeftward)
	{
		scene->getCamera()->moveSideward(-m_cameraMoveSpeed);
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
			m_cameraMoveSpeed *= 2;
			break;
		case(PageDownButton):
			m_cameraMoveSpeed /= 2;
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
		scene->getCamera()->rotateGlobalZ(dx * m_cameraRotationSpeed);
		scene->getCamera()->rotateLocalX(dy * m_cameraRotationSpeed);
	}
}
void hge::core::Director::newData(const unsigned int &size, unsigned char *const &data)
{
}
