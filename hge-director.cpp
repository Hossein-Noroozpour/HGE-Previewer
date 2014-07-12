#include "hge-director.hpp"
#include "hge-terrain-unit.hpp"
#include "hge-terrain-sun-shader.hpp"
#include "hge-protocol.hpp"
#include <thread>
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
	std::this_thread::yield();
	gldo_lock.lock();
	for(auto f : gldo)
	{
		f();
	}
	gldo.clear();
	//////////////////////////////////////////////////////////////////
	gldo_lock.unlock();
	std::this_thread::yield();
}
void hge::core::Director::buttonPressed(const HGEButton& key)
{
	(void)key;
	/// TODO: For now.
}
void hge::core::Director::buttonReleased(const HGEButton& key)
{
	(void)key;
	/// TODO: For now.
}
void hge::core::Director::mouseMoved(const float &dx, const float &dy)
{
	(void)dx;
	(void)dy;
	/// TODO: For now.
}
void hge::core::Director::newData(const unsigned int &size, unsigned char *const &data)
{
	new_data_lock.lock();
	switch(((Protocol::instructions *)data)[0])
	{
		case Protocol::new_object :
		{
			unsigned int id = ((unsigned int *)(data + sizeof(Protocol::instructions)))[0];
			object_ids.push_back(id);
			switch (((Protocol::type_id *)(data + sizeof(Protocol::instructions) + sizeof(unsigned int)))[0])
			{
				case Protocol::terrain:
				{
					auto f = [=] () -> void
					{
						auto terrain = std::shared_ptr<render::TerrainUnit>(new render::TerrainUnit(
								size - (sizeof(Protocol::instructions) + sizeof(unsigned int) + sizeof(Protocol::type_id)),
								data + sizeof(Protocol::instructions) + sizeof(unsigned int) +
								sizeof(Protocol::type_id)));
						auto terrainShader = std::shared_ptr<hge::shader::TerrainSunShader>(new hge::shader::TerrainSunShader());
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
							for(auto i = 0; i < 8; i++)
							{
								auto tex = std::shared_ptr<hge::texture::TextureUnit>(
											new hge::texture::TextureUnit(GL_TEXTURE_2D, texL + texF[i]));
								tex->load();
								terrain->addTexture(tex);
							}
						}
						scene->setTerrain(terrain);
						new_data_lock.unlock();
					};
					gldo_lock.lock();
					gldo.push_back(static_cast<std::function<void(void)>>(f));
					gldo_lock.unlock();
					break;
				}
				default:
				{
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
