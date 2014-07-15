#ifndef HGECOLLADAIMPORTER_HPP
#define	HGECOLLADAIMPORTER_HPP
#include "hge-scene-unit.hpp"
#include "hge-geometry-unit.hpp"
#include "hge-mesh-unit.hpp"
#include <rapidxml.h>
//#define HGECOLLADADEBUG
//#define HGECOLLADADEBUG1
//#define HGECOLLADADEBUG2
namespace hge
{
	namespace utility
	{
		class ColladaImporter
		{
		public:
			static std::shared_ptr<render::SceneUnit> importFromFile(const std::string& fileName);
		private:
			static std::shared_ptr<render::SceneUnit> colladaNode(rapidxml::xml_node<>* const& node);
			static std::vector<std::shared_ptr<render::GeometryUnit>> libraryGeometriesNode(rapidxml::xml_node<>* const& node, std::shared_ptr<render::SceneUnit>& scene);
			/// This function will be used for every thing that only positions, normals and texture coordinates is important for them.
			static std::shared_ptr<render::GeometryUnit> geometryNodePNT(rapidxml::xml_node<>* const& node);
			/// This function will be used for occlusion query object and every thing that only position is important for them.
			static std::shared_ptr<render::GeometryUnit> geometryNodeP  (rapidxml::xml_node<>* const& node);
			/// This function will be used for sky box and every thing that only positions and texture coordinate is important for them.
			static std::shared_ptr<render::GeometryUnit> geometryNodePT (rapidxml::xml_node<>* const& node);
			///\warning I assumed that all UVs are sorted.
			///\warning It does not support multichannel texture coordinating.
			///\TODO Support multichannel texture coordinating.
			static std::shared_ptr<render::MeshUnit> meshNodePNT(rapidxml::xml_node<>* const& node, const std::string& geoid);
			static std::shared_ptr<render::MeshUnit> meshNodePT (rapidxml::xml_node<>* const& node, const std::string& geoid);
			static std::shared_ptr<render::MeshUnit> meshNodeP  (rapidxml::xml_node<>* const& node, const std::string& geoid);
			static std::vector<GLfloat> sourceDataNode(rapidxml::xml_node<>* const& node);
			static std::vector<GLuint> sourcePolyListNode(rapidxml::xml_node<>* const& node);
		};
	}
}
#endif
