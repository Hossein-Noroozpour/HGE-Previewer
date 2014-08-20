#include "hge-director.hpp"
#include "hge-terrain-unit.hpp"
#include "hge-terrain-sun-shader.hpp"
#include "hge-protocol.hpp"
#include "hge-collada-importer.hpp"
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
	auto terrainShader = std::shared_ptr<hge::shader::TerrainSunShader>(new hge::shader::TerrainSunShader());
	std::shared_ptr<hge::render::TerrainUnit> terrain;
	//const std::string shdL = "C:\\Users\\Thany\\Documents\\Visual Studio 2012\\Projects\\HGE\\x64\\Release\\shaders\\";
	const std::string shdL = "shaders/";

	std::shared_ptr<hge::shader::SkyBoxShaderUnit> skyshd(new hge::shader::SkyBoxShaderUnit(shdL + "hge-skybox-shader-string"));

	//sconst td::string scnL = "C:\\Users\\Thany\\Documents\\Visual Studio 2012\\Projects\\HGE\\x64\\Release\\objects\\";
	const std::string scnL = "objects/";
	scene = hge::utility::ColladaImporter::importFromFile(scnL + "scene.dae");

	scene->getSky()->setShader(skyshd);

	//const std::string texL = "C:\\Users\\Thany\\Documents\\Visual Studio 2012\\Projects\\HGE\\x64\\Release\\textures\\";
	const std::string texL = "textures/";

	{
		const std::string skytexfile = texL + "sky.jpg";
		auto tex = std::shared_ptr<hge::texture::TextureUnit>(new hge::texture::TextureUnit(GL_TEXTURE_2D, skytexfile));
		scene->getSky()->setTexture(tex);
	}

	{
		const std::string fantanfile = texL + "fantan.jpg";
		auto tex = std::shared_ptr<hge::texture::TextureUnit>(new hge::texture::TextureUnit(GL_TEXTURE_2D, fantanfile));
		scene->getGeoByID("Fantan-mesh")->setTexture(tex);
		scene->getGeoByID("Fantan-mesh")->getModelMatrix()->scale(0.01);
		scene->getGeoByID("Fantan-mesh")->getModelMatrix()->translate(hge::math::Vector3D<>(1000.0f));
	}

	//const std::string datL = "C:\\Users\\Thany\\Documents\\Visual Studio 2012\\Projects\\HGE\\x64\\Release\\data\\";
	const std::string datL = "data/";
	std::fstream in (datL + "scene.hge", std::ios::binary | std::ios::in);
	if(in)
	{
		bool sameEndian = true;
		{
			uint8_t endian = 0;
			in.read((char*)&endian, 1);
			int checknum = 1;
			if(*((char *)(&checknum)) == 1)
			{/// Little Endian
				endian == 0 ? sameEndian = true : sameEndian = false;
			}
			else
			{/// Big Endian
				endian == 1 ? sameEndian = true : sameEndian = false;
			}
		}
		uint64_t objectType = 0;
		in.read((char *)&objectType, sizeof(uint64_t));
		if(!sameEndian) swapBytes((unsigned char *)&objectType, sizeof(uint64_t));
		if(objectType == 0)/// Object is terrain
		{
			uint32_t terrainAspects = 0;
			in.read((char *)&terrainAspects, sizeof(uint32_t));
			if(!sameEndian) swapBytes((unsigned char *)&terrainAspects, sizeof(uint32_t));

			double hd;
			in.read((char *)&hd, sizeof(double));
			if(!sameEndian) swapBytes((unsigned char *)&hd, sizeof(double));

			double vd;
			in.read((char *)&vd, sizeof(double));
			if(!sameEndian) swapBytes((unsigned char *)&vd, sizeof(double));
			int16_t *terrainBytes = new int16_t[terrainAspects * terrainAspects * 2];
			in.read((char *)terrainBytes, terrainAspects * terrainAspects * 2);
			{
				auto nofB = in.gcount();
				if(terrainAspects * terrainAspects * 2 != nofB)
				{
					std::cout << "Same Endian: " << sameEndian << std::endl;
					std::cout << "Number of bytes: " << nofB << std::endl;
					std::cout << "Size of array: " << terrainAspects * terrainAspects * 2 << std::endl;
					std::cout << "Terrain aspects: " << terrainAspects << std::endl;
					std::cout << "Horizontal degree: " << hd << std::endl;
					std::cout << "Vertical degree: " << vd << std::endl;
					std::cerr << "Error in reading hge file!" << std::endl;
					HGEPRINTCODELINE
					std::terminate();
				}
			}
			if(!sameEndian)
			{ /// Little-Endian
				unsigned char *bytes = (unsigned char *)terrainBytes;
				for(unsigned int i = 0; i < terrainAspects * terrainAspects * 2; i += 2)
				{
					unsigned char tmpch = bytes[i];
					bytes[i] = bytes[i + 1];
					bytes[i + 1] = tmpch;
				}
			}
			terrain = std::shared_ptr<hge::render::TerrainUnit>(
						new hge::render::TerrainUnit(terrainBytes, terrainAspects, vd, hd));
		}
	}
	else
	{
		std::cerr << "Error in opening file!" << std::endl;
		HGEPRINTCODELINE ;
		std::terminate();
	}
	terrain->setShader(terrainShader);
	{
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
