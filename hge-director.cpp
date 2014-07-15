#include "hge-director.hpp"
#include "hge-terrain-unit.hpp"
#include "hge-terrain-sun-shader.hpp"
#include "hge-protocol.hpp"
#include <thread>
#include <iostream>
#define HGEPRINTCODELINE std::cout << "Debugging: file:" << __FILE__ << " line:" << __LINE__ << std::endl << std::flush;
hge::core::Director::Director():
	goForward(false),
	goDownward(false),
	goRightward(false),
	goLeftward(false),
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
void hge::core::Director::initialize()
{
	scene = std::shared_ptr<render::SceneUnit>(new render::SceneUnit());
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
		default:
			break;
	}
}
void hge::core::Director::mouseMoved(const float &dx, const float &dy)
{
	scene->getCamera()->rotateGlobalZ(dx * m_cameraRotationSpeed);
	scene->getCamera()->rotateLocalX(dy * m_cameraRotationSpeed);
}
void hge::core::Director::newData(const unsigned int &size, unsigned char *const &data)
{
	new_data_lock.lock();
	switch(((Protocol::instructions_type *)data)[0])
	{
		case Protocol::new_object :
		{
			const unsigned int id_index = sizeof(Protocol::instructions);
			const unsigned int type_index = id_index + sizeof(Protocol::id_type);
			const unsigned int data_index = type_index + sizeof(Protocol::object_type_id_type);
			Protocol::id_type id = ((Protocol::id_type *)(&(data[id_index])))[0];
			object_ids.push_back(id);
			switch (((Protocol::object_type_id_type *)(&(data[type_index])))[0])
			{
				case Protocol::terrain:
				{
					auto f = [=] () -> void
					{
						std::shared_ptr<render::TerrainUnit> terrain(new render::TerrainUnit(size - data_index, data + data_index));
						std::shared_ptr<hge::shader::TerrainSunShader> terrainShader(new hge::shader::TerrainSunShader());
						terrain->setShader(terrainShader);
						{
							const std::string texL = "textures/";
							std::string texF [] =
							{
								"ice.jpg",
								"snow.jpg",
								"stone.jpg",
								"grass1.jpg",
								"grass2.jpg",
								"grass3.jpg",
								"sand.jpg",
								"sea.jpg"
							};
							for(unsigned int i = 0; i < 8; i++)
							{
								std::shared_ptr<hge::texture::TextureUnit> tex(new hge::texture::TextureUnit(GL_TEXTURE_2D, texL + texF[i]));
								tex->load();
								terrain->addTexture(tex);
							}
						}
						scene->setTerrain(terrain);
						new_data_lock.unlock();
						delete [] data;
					};
					gldo_lock.lock();
					gldo.push_back(static_cast<std::function<void(void)>>(f));
					gldo_lock.unlock();
					break;
				}
				default:
				{
					HGEPRINTCODELINE HGEPRINTCODELINE HGEPRINTCODELINE
					break;
				}
			}
			break;
		}
		default:
		{
			break;
		}
	}
}
