#include "hge-collada-importer.hpp"
#include "hge-vertex-data.hpp"
#include "hge-skybox-unit.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#define HGEPRINTCODELINE std::cout << "Debugging: file:" << __FILE__ << " line:" << __LINE__ << std::endl << std::flush;
std::shared_ptr<hge::render::SceneUnit> hge::utility::ColladaImporter::importFromFile(const std::string& fileName)
{
	std::shared_ptr<render::SceneUnit> result;
	std::ifstream in(fileName);
	std::string xmlText;
	if(in)
	{
		in.seekg(0, std::ios::end);
		xmlText.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&xmlText[0], xmlText.size());
		in.close();

	}
	else
	{
		std::cerr << "Error in reading Collada File(" + fileName + ")!" << std::endl;
		std::terminate();
	}

	auto doc = std::shared_ptr<rapidxml::xml_document<>>(new rapidxml::xml_document<>());
	doc->parse<0>(const_cast<char *>(xmlText.c_str()));
	for(auto node = doc->first_node(); node != 0; node = node->next_sibling())
	{
		if(std::string(node->name()) == std::string("COLLADA"))
		{
#ifdef HGECOLLADADEBUG
			std::cout << "I've seen collada." << std::endl;
			HGEPRINTCODELINE
#endif
			result = colladaNode(node);
		}
	}
	return result;
}
std::shared_ptr<hge::render::SceneUnit> hge::utility::ColladaImporter::colladaNode(rapidxml::xml_node<>* const& node)
{
	std::shared_ptr<render::SceneUnit> result(new render::SceneUnit());
	std::vector<std::shared_ptr<render::GeometryUnit>> geometries;
	std::string libraryGeometriesString = "library_geometries";
	for(auto child = node->first_node(); child != 0; child = child->next_sibling())
	{
		if(child->name() == libraryGeometriesString)
		{
#ifdef HGECOLLADADEBUG
			std::cout << "I've seen library_geometries." << std::endl;
			HGEPRINTCODELINE
#endif
			geometries = libraryGeometriesNode(child, result);
		}
	}
	for(auto geo : geometries)
	{
		result->addGeometry(geo);
	}
	return result;
}
std::vector<std::shared_ptr<hge::render::GeometryUnit>> hge::utility::ColladaImporter::libraryGeometriesNode(rapidxml::xml_node<>* const& node, std::shared_ptr<render::SceneUnit> &scene)
{
	std::vector<std::shared_ptr<render::GeometryUnit>> result;
	const std::string geometryString("geometry");
	for(auto child = node->first_node(); child != 0; child = child->next_sibling())
	{
		if(geometryString == child->name())
		{
			const std::string namestr = child->first_attribute("name", 4)->value();
#ifdef HGECOLLADADEBUG
			std::cout << "I've seen geometry." << std::endl;
			HGEPRINTCODELINE
#endif
			if(namestr.find("SkyBox") == 0)
			{
#ifdef HGECOLLADADEBUG
				std::cout << "I've seen a SkyBox." << std::endl;
				HGEPRINTCODELINE
#endif
				auto geo = geometryNodePT(child);
				std::shared_ptr<render::SkyBoxUnit> sky(new render::SkyBoxUnit(geo->getMesh()));
				scene->addSky(sky);
			}
			else if(namestr.find("OcclusionTest") != 0)
			{
				auto geo = geometryNodePNT(child);
				const std::string occQryStr ="OcclusionTest" +  namestr;
				for(auto occchild = node->first_node(); occchild != 0; occchild = occchild->next_sibling())
				{
					if(geometryString == occchild->name())
					{
						if(occQryStr == occchild->first_attribute("name", 4)->value())
						{
							auto occgeo = geometryNodeP(occchild);
							geo->setOcclusionQueryMesh(occgeo->getMesh());
							break;
						}
					}
				}
				result.push_back(geo);
			}
		}
	}
	return result;
}
std::shared_ptr<hge::render::GeometryUnit> hge::utility::ColladaImporter::geometryNodePNT(rapidxml::xml_node<>* const& node)
{
	const std::string idstr  (node->first_attribute("id"  , 2)->value());
	const std::string namestr(node->first_attribute("name", 4)->value());
	std::shared_ptr<render::GeometryUnit> result(new render::GeometryUnit(idstr, namestr));
	const std::string meshString("mesh");
	for(auto child = node->first_node(); child != 0; child = child->next_sibling())
	{
		if(meshString == child->name())
		{
#ifdef HGECOLLADADEBUG
			std::cout << "I've seen mesh for positions, normals and texture coordinates." << std::endl;
			HGEPRINTCODELINE
#endif
			auto mesh = meshNodePNT(child, idstr);
			result->setMesh(mesh);
		}
	}
	return result;
}
std::shared_ptr<hge::render::GeometryUnit> hge::utility::ColladaImporter::geometryNodeP(rapidxml::xml_node<>* const& node)
{
	const std::string idstr  (node->first_attribute("id"  , 2)->value());
	const std::string namestr(node->first_attribute("name", 4)->value());
	std::shared_ptr<render::GeometryUnit> result(new render::GeometryUnit(idstr, namestr));
	const std::string meshString("mesh");
	for(auto child = node->first_node(); child != 0; child = child->next_sibling())
	{
		if(meshString == child->name())
		{
#ifdef HGECOLLADADEBUG
			std::cout << "I've seen mesh for positions." << std::endl;
			HGEPRINTCODELINE
#endif
			auto mesh = meshNodeP(child, idstr);
			result->setMesh(mesh);
		}
	}
	return result;
}
std::shared_ptr<hge::render::GeometryUnit> hge::utility::ColladaImporter::geometryNodePT(rapidxml::xml_node<>* const& node)
{
	const std::string idstr  (node->first_attribute("id"  , 2)->value());
	const std::string namestr(node->first_attribute("name", 4)->value());
	std::shared_ptr<render::GeometryUnit> result(new render::GeometryUnit(idstr, namestr));
	const std::string meshString("mesh");
	for(auto child = node->first_node(); child != 0; child = child->next_sibling())
	{
		if(meshString == child->name())
		{
#ifdef HGECOLLADADEBUG
			std::cout << "I've seen mesh for positions and texture coordinates." << std::endl;
			HGEPRINTCODELINE
#endif
			auto mesh = meshNodePT(child, idstr);
			result->setMesh(mesh);
		}
	}
	return result;
}
std::shared_ptr<hge::render::MeshUnit> hge::utility::ColladaImporter::meshNodePNT(rapidxml::xml_node<>* const& node, const std::string& geoid)
{
	const std::string positionStr    = geoid + "-positions";
	const std::string normalStr      = geoid + "-normals";
	const std::string uvStr          = geoid + "-map-";
	const std::string sourceString   = "source";
	const std::string polylistString = "polylist";
	std::vector<GLfloat>              positions;
	std::vector<GLfloat>              normals;
	std::vector<std::vector<GLfloat>> uvs;
	std::vector<GLuint>               polylist;
	std::set<hge::core::VertexData, hge::core::VertexData::Compare> vertexSet;
	for(auto child = node->first_node(); child != 0; child = child->next_sibling())
	{
		if(sourceString == child->name())
		{
			const std::string idstr(child->first_attribute("id", 2)->value());
#ifdef HGECOLLADADEBUG
			std::cout << "I've seen source." << std::endl;
			HGEPRINTCODELINE
#endif
			if(idstr == positionStr)
			{
				positions = sourceDataNode(child);
#ifdef HGECOLLADADEBUG1
				std::cout << "Positions: ";
				for(auto i : positions)
				{
					std::cout << i << " ";
				}
				std::cout << std::endl;
				HGEPRINTCODELINE
#endif
			}
			else if(idstr == normalStr)
			{
				normals = sourceDataNode(child);
#ifdef HGECOLLADADEBUG1
				std::cout << "Normals: ";
				for(auto i : normals)
				{
					std::cout << i << " ";
				}
				std::cout << std::endl;
				HGEPRINTCODELINE
#endif
			}
			else if(idstr.find(uvStr) == 0)
			{
				uvs.push_back(sourceDataNode(child));
#ifdef HGECOLLADADEBUG1
				std::cout << "UVs: ";
				for(auto i : uvs[0])
				{
					std::cout << i << " ";
				}
				std::cout << "Size of UVs: " << uvs[0].size() << std::endl;
				HGEPRINTCODELINE
#endif
			}
		}
		else if(polylistString == child->name())
		{
			polylist = sourcePolyListNode(child);
#ifdef HGECOLLADADEBUG1
				std::cout << "polylist: ";
				for(auto i : polylist)
				{
					std::cout << i << " ";
				}
				std::cout << std::endl;
				HGEPRINTCODELINE
#endif
		}
	}
	unsigned int j = 0;
	for(unsigned int i = 0, tmpint = 0; i < polylist.size(); j++)
	{
		core::VertexData tmpv;
		tmpint = polylist[i++] * 3;
		tmpv.px = positions[tmpint++];
		tmpv.py = positions[tmpint++];
		tmpv.pz = positions[tmpint];
		tmpint = polylist[i++] * 3;
		tmpv.nx = normals[tmpint++];
		tmpv.ny = normals[tmpint++];
		tmpv.nz = normals[tmpint];
		tmpint = polylist[i++] * 2;
		tmpv.ux = uvs[0][tmpint++];
		tmpv.uy = uvs[0][tmpint];
		auto iter = vertexSet.find(tmpv);
		if(iter == vertexSet.end())
		{
			tmpv.indices.push_back(j);
			vertexSet.insert(tmpv);
		}
		else
		{
			const_cast<std::vector<unsigned int>*>(&(iter->indices))->push_back(j);
		}
	}
#ifdef HGECOLLADADEBUG
	std::cout << "Vertices size: " << vertexSet.size() << std::endl;
	std::cout << "Indices size: " << j << std::endl;
	HGEPRINTCODELINE
#endif
#ifdef HGECOLLADADEBUG1
	for(auto i : vertexSet)
	{
		i.print();
	}
	HGEPRINTCODELINE
#endif
	GLuint *indices = new GLuint[j];
	GLfloat *vertices = new GLfloat[vertexSet.size() * 8];
	unsigned int tmpint = 0, realIndex = 0;
	for(auto iter: vertexSet)
	{
		vertices[tmpint++] = iter.px;
		vertices[tmpint++] = iter.py;
		vertices[tmpint++] = iter.pz;
		vertices[tmpint++] = iter.nx;
		vertices[tmpint++] = iter.ny;
		vertices[tmpint++] = iter.nz;
		vertices[tmpint++] = iter.ux;
		vertices[tmpint++] = iter.uy;
		for(auto index: iter.indices)
		{
			indices[index] = realIndex;
		}
		++realIndex;
	}
	std::shared_ptr<render::MeshUnit>
		mesh(new render::MeshUnit(vertices, indices, GLuint(vertexSet.size() * 8 * sizeof(GLfloat)), GLuint(vertexSet.size()), j * sizeof(GLuint)));
	return mesh;
}
std::shared_ptr<hge::render::MeshUnit> hge::utility::ColladaImporter::meshNodePT(rapidxml::xml_node<>* const& node, const std::string& geoid)
{
	const std::string positionStr    = geoid + "-positions";
	const std::string uvStr          = geoid + "-map-";
	const std::string sourceString   = "source";
	const std::string polylistString = "polylist";
	std::vector<GLfloat>              positions;
	std::vector<std::vector<GLfloat>> uvs;
	std::vector<GLuint>               polylist;
	std::set<hge::core::VertexData, hge::core::VertexData::Compare> vertexSet;
	for(auto child = node->first_node(); child != 0; child = child->next_sibling())
	{
		if(sourceString == child->name())
		{
			const std::string idstr  (child->first_attribute("id"  , 2)->value());
#ifdef HGECOLLADADEBUG
			std::cout << "I've seen source." << std::endl;
			HGEPRINTCODELINE
#endif
			if(idstr == positionStr)
			{
				positions = sourceDataNode(child);
#ifdef HGECOLLADADEBUG1
				std::cout << "Positions: ";
				for(auto i : positions)
				{
					std::cout << i << " ";
				}
				std::cout << std::endl;
				HGEPRINTCODELINE
#endif
			}
			else if(idstr.find(uvStr) == 0)
			{
				uvs.push_back(sourceDataNode(child));
#ifdef HGECOLLADADEBUG1
				std::cout << "UVs: ";
				for(auto i : uvs[0])
				{
					std::cout << i << " ";
				}
				std::cout << "Size of UVs: " << uvs[0].size() << std::endl;
				HGEPRINTCODELINE
#endif
			}
		}
		else if(polylistString == child->name())
		{
			polylist = sourcePolyListNode(child);
#ifdef HGECOLLADADEBUG1
				std::cout << "polylist: ";
				for(auto i : polylist)
				{
					std::cout << i << " ";
				}
				std::cout << std::endl;
				HGEPRINTCODELINE
#endif
		}
	}
	unsigned int j = 0;
	for(unsigned int i = 0, tmpint = 0; i < polylist.size(); j++)
	{
		core::VertexData tmpv;
		tmpint = polylist[i++] * 3;
		tmpv.px = positions[tmpint++];
		tmpv.py = positions[tmpint++];
		tmpv.pz = positions[tmpint];
		i++;
		tmpv.nx = 0.0f;
		tmpv.ny = 0.0f;
		tmpv.nz = 0.0f;
		tmpint = polylist[i++] * 2;
		tmpv.ux = uvs[0][tmpint++];
		tmpv.uy = uvs[0][tmpint];
		auto iter = vertexSet.find(tmpv);
		if(iter == vertexSet.end())
		{
			tmpv.indices.push_back(j);
			vertexSet.insert(tmpv);
		}
		else
		{
			const_cast<std::vector<unsigned int>*>(&(iter->indices))->push_back(j);
		}
	}
#ifdef HGECOLLADADEBUG
	std::cout << "Vertices size: " << vertexSet.size() << std::endl;
	std::cout << "Indices size: " << j << std::endl;
	HGEPRINTCODELINE
#endif
#ifdef HGECOLLADADEBUG1
	for(auto i : vertexSet)
	{
		i.print();
	}
	HGEPRINTCODELINE
#endif
	GLuint *indices = new GLuint[j];
	GLfloat *vertices = new GLfloat[vertexSet.size() * 5];
	unsigned int tmpint = 0, realIndex = 0;
	for(auto iter: vertexSet)
	{
		vertices[tmpint++] = iter.px;
		vertices[tmpint++] = iter.py;
		vertices[tmpint++] = iter.pz;
		vertices[tmpint++] = iter.ux;
		vertices[tmpint++] = iter.uy;
		for(auto index: iter.indices)
		{
			indices[index] = realIndex;
		}
		++realIndex;
	}
	std::shared_ptr<render::MeshUnit>
		mesh(new render::MeshUnit(vertices, indices, GLuint(vertexSet.size() * 5 * sizeof(GLfloat)), GLuint(vertexSet.size()), j * sizeof(GLuint)));
	return mesh;
}
std::shared_ptr<hge::render::MeshUnit> hge::utility::ColladaImporter::meshNodeP(rapidxml::xml_node<>* const& node, const std::string& geoid)
{
	const std::string positionStr    = geoid + "-positions";
	const std::string sourceString   = "source";
	const std::string polylistString = "polylist";
	std::vector<GLfloat>              positions;
	std::vector<GLuint>               polylist;
	std::set<hge::core::VertexData, hge::core::VertexData::Compare> vertexSet;
	for(auto child = node->first_node(); child != 0; child = child->next_sibling())
	{
		if(sourceString == child->name())
		{
			const std::string idstr(child->first_attribute("id", 2)->value());
#ifdef HGECOLLADADEBUG
			std::cout << "I've seen source." << std::endl;
			HGEPRINTCODELINE
#endif
			if(idstr == positionStr)
			{
				positions = sourceDataNode(child);
#ifdef HGECOLLADADEBUG1
				std::cout << "Positions: ";
				for(auto i : positions)
				{
					std::cout << i << " ";
				}
				std::cout << std::endl;
				HGEPRINTCODELINE
#endif
			}
		}
		else if(polylistString == child->name())
		{
			polylist = sourcePolyListNode(child);
#ifdef HGECOLLADADEBUG1
				std::cout << "polylist: ";
				for(auto i : polylist)
				{
					std::cout << i << " ";
				}
				std::cout << std::endl;
				HGEPRINTCODELINE
#endif
		}
	}
	unsigned int j = 0;
	for(unsigned int i = 0, tmpint = 0; i < polylist.size(); i += 3, j++)
	{
		core::VertexData tmpv;
		tmpint = polylist[i] * 3;
		tmpv.px = positions[tmpint++];
		tmpv.py = positions[tmpint++];
		tmpv.pz = positions[tmpint];
		tmpv.nx = 0.0f;
		tmpv.ny = 0.0f;
		tmpv.nz = 0.0f;
		tmpv.ux = 0.0f;
		tmpv.uy = 0.0f;
		auto iter = vertexSet.find(tmpv);
		if(iter == vertexSet.end())
		{
			tmpv.indices.push_back(j);
			vertexSet.insert(tmpv);
		}
		else
		{
			const_cast<std::vector<unsigned int>*>(&(iter->indices))->push_back(j);
		}
	}
#ifdef HGECOLLADADEBUG
	std::cout << "Vertices size: " << vertexSet.size() << std::endl;
	std::cout << "Indices size: " << j << std::endl;
	HGEPRINTCODELINE
#endif
#ifdef HGECOLLADADEBUG1
	for(auto i : vertexSet)
	{
		i.print();
	}
	HGEPRINTCODELINE
#endif
	GLuint *indices = new GLuint[j];
	GLfloat *vertices = new GLfloat[vertexSet.size() * 3];
	unsigned int tmpint = 0, realIndex = 0;
	for(auto iter: vertexSet)
	{
		vertices[tmpint++] = iter.px;
		vertices[tmpint++] = iter.py;
		vertices[tmpint++] = iter.pz;
		for(auto index: iter.indices)
		{
			indices[index] = realIndex;
		}
		++realIndex;
	}
	std::shared_ptr<render::MeshUnit>
		mesh(new render::MeshUnit(vertices, indices, GLuint(vertexSet.size() * 3 * sizeof(GLfloat)), GLuint(vertexSet.size()), j * sizeof(GLuint)));
	return mesh;
}
std::vector<GLfloat> hge::utility::ColladaImporter::sourceDataNode(rapidxml::xml_node<>* const& node)
{
	std::vector<GLfloat> result;
	const std::string floatArrayString = "float_array";
	for(auto child = node->first_node(); child != 0; child = child->next_sibling())
	{
		if(floatArrayString == child->name())
		{
			unsigned long numVertices = std::stoul(child->first_attribute("count", 5)->value());
			std::string tmpstr = child->value();
			std::stringstream ssin(tmpstr);
			for(unsigned long i = 0; i < numVertices; i++)
			{
				ssin >> tmpstr;
				result.push_back(std::stof(tmpstr));
			}
		}
	}
	return result;
}
std::vector<GLuint> hge::utility::ColladaImporter::sourcePolyListNode(rapidxml::xml_node<>* const& node)
{
	std::vector<GLuint> result;
	unsigned int numberOfFaces = std::stoul(node->first_attribute("count", 5)->value());
	const std::string vCountString = "vcount";
	const std::string pString = "p";
	for(auto child = node->first_node(); child != 0; child = child->next_sibling())
	{
		if(vCountString == child->name())
		{
			std::stringstream ssin(child->value());
			std::string tempstr;
			for(unsigned int i = 0; i < numberOfFaces; ++i)
			{
				ssin >> tempstr;
				if(std::stoi(tempstr) != 3)
				{
					std::cerr << "Error: Critical. Line: " << __LINE__;
					std::cerr << ", File: " << __FILE__ << std::endl;
					std::cerr << "Object must be triangular!" << std::endl;
					std::terminate();
				}
			}
		}
		else if(pString == child->name())
		{
			std::string tempstr;
			std::stringstream ssin(child->value());
			for(unsigned int i = 0; i < numberOfFaces * 9; ++i)
			{
				ssin >> tempstr;
				result.push_back(std::stoi(tempstr));
			}
		}
	}
	return result;
}
