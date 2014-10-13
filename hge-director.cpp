#include "hge-director.hpp"
#include "hge-texture-unit.hpp"
#include "hge-terrain-unit.hpp"
#include "hge-terrain-sun-shader.hpp"
#include "hge-protocol.hpp"
#include "hge-skybox-shader-unit.hpp"
#include "hge-resource-manager.hpp"
#include <thread>
#include <iostream>
#include <fstream>
#define HGEPRINTCODELINE std::cout << "Debugging: file:" << __FILE__ << " line:" << __LINE__ << std::endl << std::flush;
hge::core::Director::Director() :
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
	/*
	/// testing part
	render::SceneUnit::defaultShader =
		std::shared_ptr<hge::shader::SunShader>(new hge::shader::SunShader());
	render::SceneUnit::defaultTexture =
		std::shared_ptr<hge::texture::TextureUnit>(new hge::texture::TextureUnit(GLenum(GL_TEXTURE_2D),
#ifdef __unix__
		std::string("/home/thany/Pictures/HGE-Logo1024x1024.png")));
#else
		std::string("C:\\Temporary\\Pictures\\Hulixerian\\HGE-Logo1024x1024.png")));
#endif
	render::SceneUnit::occlusionQueryShader =
		std::shared_ptr<hge::shader::WhiteShader>(new hge::shader::WhiteShader());
	/// end of testing part
#ifdef __unix__
	scene = ResourceManager::importScene("/home/thany/Programming/QtCreator/Files/hge-sample.hge");
#else
	scene = ResourceManager::importScene("C:\\Users\\Thany Hulixer\\Documents\\Projects\\HGE\\HGE-Builder\\hge-sample.hge");
#endif
	scene->getTerrain()->setShader(std::shared_ptr<shader::TerrainSunShader>(new shader::TerrainSunShader()));*/





	ws = std::shared_ptr<shader::WhiteShader>(new shader::WhiteShader);
	float *v = new float[3*3];
	GLuint *i = new GLuint[3];
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
	v[3] = 1.0f;
	v[4] = 1.0f;
	v[5] = 0.0f;
	v[6] = -1.0f;
	v[7] = 1.0f;
	v[8] = 0.0f;
	i[0] = 0;
	i[1] = 1;
	i[2] = 2;
	m = std::shared_ptr<render::MeshUnit>(new render::MeshUnit(v, i, 9 * sizeof(GLfloat), 3, 3 * sizeof(GLuint)));
}
void hge::core::Director::update()
{
	/*gldo_lock.lock();
	for (auto f : gldo)
	{
		f();
	}
	gldo.clear();
	gldo_lock.unlock();
	if (goForward)
	{
		scene->getCamera()->moveForward(cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	if (goDownward)
	{
		scene->getCamera()->moveForward(-cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	if (goRightward)
	{
		scene->getCamera()->moveSideward(cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	if (goLeftward)
	{
		scene->getCamera()->moveSideward(-cameraMoveSpeed);
		//terrainShader->setCameraPositionRelativeToModel(scene->getCamera()->getLocation());
	}
	scene->draw();
	////////////////////////////////////////////////////////*/




	if (goForward)
	{
		c.moveForward(cameraMoveSpeed);
	}
	if (goDownward)
	{
		c.moveForward(-cameraMoveSpeed);
	}
	if (goRightward)
	{
		c.moveSideward(cameraMoveSpeed);
	}
	if (goLeftward)
	{
		c.moveSideward(-cameraMoveSpeed);
	}
	m->bindVBO();
	ws->use();
	auto mvp = (p.getMatrix() * c.getMatrix()) * mm.getConstRotateScaleTranslateMatrix();
	ws->setModelViewProjectionMatrix(mvp);
	m->bindIBO();
	m->draw();



}
void hge::core::Director::buttonPressed(const HGEButton& key)
{
	switch (key)
	{
	case(F1KeyButton) :
		if (wireframeMode)
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
	case(PageUpButton) :
		cameraMoveSpeed *= 2;
		break;
	case(PageDownButton) :
		cameraMoveSpeed /= 2;
		break;
	case(wKeyButton) :
		goForward = true;
		break;
	case(sKeyButton) :
		goDownward = true;
		break;
	case(aKeyButton) :
		goLeftward = true;
		break;
	case(dKeyButton) :
		goRightward = true;
		break;
	case(middleMouseButton) :
		rotateOn = true;
		break;
	default:
		break;
	}
}
void hge::core::Director::buttonReleased(const HGEButton& key)
{
	switch (key)
	{
	case(wKeyButton) :
		goForward = false;
		break;
	case(sKeyButton) :
		goDownward = false;
		break;
	case(aKeyButton) :
		goLeftward = false;
		break;
	case(dKeyButton) :
		goRightward = false;
		break;
	case(middleMouseButton) :
		rotateOn = false;
		break;
	default:
		break;
	}
}
void hge::core::Director::mouseMoved(const float &dx, const float &dy)
{
	if (rotateOn)
	{
		//std::cout << "sdfssdfsfsdfsfsdfdsfs";
		/*scene->getCamera()->rotateGlobalZ(dx * cameraRotationSpeed);
		scene->getCamera()->rotateLocalX(dy * cameraRotationSpeed);*/



		c.rotateGlobalZ(dx * cameraRotationSpeed);
		c.rotateLocalX(dy * cameraRotationSpeed); 
	}
}
