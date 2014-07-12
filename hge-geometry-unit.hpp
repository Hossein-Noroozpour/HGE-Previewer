#ifndef HGE_GEOMETRY_UNIT_HPP_
#define HGE_GEOMETRY_UNIT_HPP_
#include <string.h>
#include "hge-mesh-unit.hpp"
#include "hge-model-unit.hpp"
#include "hge-shader-unit.hpp"
#include "hge-texture-unit.hpp"
#include "hge-math.hpp"
#include <memory>
//#define HGEGEOMETRYDEBUGMVPVALUECHECK
#define HGEGEOMETRYNUMBEROFQUERIES 2
#define HGEGEOMETRYOCCLUSIONQUERYINDEX 0
#define HGEGEOMETRYPIXELSPASSED 1
namespace hge
{
	namespace render
	{
		class GeometryUnit
		{
		public:
			GeometryUnit(const std::string& id, const std::string& name);
			~GeometryUnit();
			void setMesh(const std::shared_ptr<MeshUnit>& m);
			void setOcclusionQueryMesh(const std::shared_ptr<MeshUnit>& m);
			void draw();
			void occlusionQuery(const math::Matrix4D<>& vp);
			void occlusionQueryStarter(const math::Matrix4D<>& vp);
			bool isThisYourID(const std::string& idstr);
			void setShader(const std::shared_ptr<shader::ShaderUnit>& shader);
			void setOcclusionQueryShader(const std::shared_ptr<shader::ShaderUnit>& shader);
			void setTexture(const std::shared_ptr<texture::TextureUnit>& texture);
			math::ModelUnit* getModelMatrix();
			std::shared_ptr<MeshUnit> getMesh();
		private:
			GLuint queries[HGEGEOMETRYNUMBEROFQUERIES];
			std::string id;
			std::string name;
			math::Matrix4D<> mvp;
			math::ModelUnit modelMatrix;
			std::shared_ptr<MeshUnit> mesh;
			std::shared_ptr<MeshUnit> occlusionQueryMesh;
			std::shared_ptr<shader::ShaderUnit> shader;
			std::shared_ptr<shader::ShaderUnit> occlusionQueryShader;
			std::shared_ptr<texture::TextureUnit> texture;
		};
	}
}
#endif
