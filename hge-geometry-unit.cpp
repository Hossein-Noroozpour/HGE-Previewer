#include "hge-geometry-unit.hpp"
#include <iostream>
#define HGEPRINTCODELINE std::cout << "Debugging: file:" << __FILE__ << " line:" << __LINE__ << std::endl << std::flush;
hge::render::GeometryUnit::GeometryUnit()
#ifdef HGE_BASIC_QUERY_SUPPORT
	:mvp(math::Matrix4D<>(1.0f))
#endif
{
#ifdef HGE_BASIC_QUERY_SUPPORT
	glGenQueries(HGEGEOMETRYNUMBEROFQUERIES, queries);
#endif
}
hge::render::GeometryUnit::~GeometryUnit()
{
#ifdef HGE_BASIC_QUERY_SUPPORT
	glDeleteQueries(HGEGEOMETRYNUMBEROFQUERIES, queries);
#endif
}
void hge::render::GeometryUnit::setMesh(const std::shared_ptr<hge::render::MeshUnit>& m)
{
	mesh = m;
}
#ifdef HGE_BASIC_QUERY_SUPPORT
void hge::render::GeometryUnit::draw()
{
#else
void hge::render::GeometryUnit::draw(const math::Matrix4D<> &vp)
{
#endif
#ifdef HGE_BASIC_QUERY_SUPPORT
#ifdef HGE_CONDITIONAL_RENDERING_SUPPORT
	glBeginConditionalRender(queries[HGEGEOMETRYOCCLUSIONQUERYINDEX], GL_QUERY_WAIT);
#else
	GLuint query_result = 0;
	glGetQueryObjectuiv(queries[HGEGEOMETRYOCCLUSIONQUERYINDEX], GL_QUERY_RESULT, &query_result);
	if(query_result == 0)
	{
		return;
	}
#endif
#endif
	//std::cout << "Render Testing." << std::endl;
	mesh->bindVBO();
	shader->use();
	texture->bind(GL_TEXTURE0);
	shader->setModelMatrix(modelMatrix.getConstRotateMatrix());
#ifndef HGE_BASIC_QUERY_SUPPORT
	auto mvp = vp * modelMatrix.getConstRotateScaleTranslateMatrix();
#endif
	shader->setModelViewProjectionMatrix(mvp);
	mesh->bindIBO();
	mesh->draw();
#ifdef HGE_BASIC_QUERY_SUPPORT
#ifdef HGE_CONDITIONAL_RENDERING_SUPPORT
	glEndConditionalRender();
#endif
#endif
}
#ifdef HGE_BASIC_QUERY_SUPPORT
void hge::render::GeometryUnit::occlusionQuery(const math::Matrix4D<> &vp)
{
	mvp = vp * modelMatrix.getConstRotateScaleTranslateMatrix();
	glBeginQuery(GL_ANY_SAMPLES_PASSED, queries[HGEGEOMETRYOCCLUSIONQUERYINDEX]);
	occlusionQueryShader->setModelViewProjectionMatrix(mvp);
	occlusionQueryMesh->bindTotal();
	occlusionQueryMesh->draw();
	glEndQuery(GL_ANY_SAMPLES_PASSED);
}
void hge::render::GeometryUnit::occlusionQueryStarter(const math::Matrix4D<> &vp)
{
	mvp = vp * modelMatrix.getConstRotateScaleTranslateMatrix();
	glBeginQuery(GL_ANY_SAMPLES_PASSED, queries[HGEGEOMETRYOCCLUSIONQUERYINDEX]);
	occlusionQueryMesh->bindTotal();
	occlusionQueryShader->use();
	occlusionQueryShader->setModelViewProjectionMatrix(mvp);
	occlusionQueryMesh->draw();
	glEndQuery(GL_ANY_SAMPLES_PASSED);
	//std::cout << "dffvsfdfsvdfdfdgdfgdgd";
}
#endif
void hge::render::GeometryUnit::setShader(const std::shared_ptr<shader::ShaderUnit>& sh)
{
	shader = sh;
}
#ifdef HGE_BASIC_QUERY_SUPPORT
void hge::render::GeometryUnit::setOcclusionQueryShader(const std::shared_ptr<shader::ShaderUnit>& sh)
{
	occlusionQueryShader = sh;
}
#endif
void hge::render::GeometryUnit::setTexture(const std::shared_ptr<texture::TextureUnit>& texture)
{
	this->texture = texture;
}
hge::math::ModelUnit* hge::render::GeometryUnit::getModelMatrix()
{
	return &modelMatrix;
}
std::shared_ptr<hge::render::MeshUnit> hge::render::GeometryUnit::getMesh()
{
	return mesh;
}
#ifdef HGE_BASIC_QUERY_SUPPORT
void hge::render::GeometryUnit::setOcclusionQueryMesh(const std::shared_ptr<MeshUnit> &m)
{
	occlusionQueryMesh = m;
}
#endif
void hge::render::GeometryUnit::setData(std::istream &stream, const bool &endianCompatible)
{
#define HGE_READ_BTYPE(t) stream.read((char *)(&t), sizeof t);if (!endianCompatible) swapObject((char *)(&t), sizeof t)
#ifdef HGE_TEST_MODE
#define HGE_TEST_FORMAT if (stream.eof()) { std::cerr << __FILE__ << ": Format error!" << std::endl; HGE_PRINT_CODE_LINE; }
#define HGE_READ_STRING(s)\
	{\
		core::Protocol::Types::StringLengthType len;\
		HGE_READ_BTYPE(len);\
		std::cout << __FILE__ << ": String length: " << len << std::endl;\
		char *str = new char[len + 1];\
		stream.read(str, len);\
		str[len] = 0;\
		s = std::string(str, len);\
		std::cout << __FILE__ << ": String is: " << str << std::endl;\
		delete [] str;\
	}
#else
#define HGE_READ_STRING(s)\
	{\
		core::Protocol::Types::StringLengthType len;\
		HGE_READ_BTYPE(len);\
		char *str = new char[len + 1];\
		stream.read(str, len);\
		str[len] = 0;\
		s = std::string(str, len);\
		delete [] str;\
	}
#define HGE_TEST_FORMAT ;
#endif

	/// ID
	std::string idStr;
	HGE_READ_STRING(idStr);
	HGE_TEST_FORMAT;
	/// Name
	std::string nameStr;
	HGE_READ_STRING(nameStr);
	HGE_TEST_FORMAT;
	/// ID
	HGE_READ_BTYPE(id);
#ifdef HGE_TEST_MODE
	std::cout << __FILE__ << ": ID number is: " << id << std::endl;
#endif
	/// Mesh
	mesh = std::shared_ptr<MeshUnit>(new MeshUnit());
	mesh->setData(stream, endianCompatible);
	HGE_TEST_FORMAT;
	math::Matrix4D<> meshMatrix;
	meshMatrix.setData(stream, endianCompatible);
	HGE_TEST_FORMAT;
	// Occlusion mesh
	core::Protocol::Types::HgeBoolean hasOcc;
	HGE_READ_BTYPE(hasOcc);
	if (core::Protocol::Values::HgeTrue == hasOcc)
	{
		occlusionQueryMesh = std::shared_ptr<MeshUnit>(new MeshUnit());
		occlusionQueryMesh->setData(stream, endianCompatible);
		HGE_TEST_FORMAT;
		math::Matrix4D<> occlusionQueryMeshMatrix;
		occlusionQueryMeshMatrix.setData(stream, endianCompatible);
		HGE_TEST_FORMAT;
	}
}
void hge::render::GeometryUnit::setDataId(const core::Protocol::Types::IdType &id)
{
	this->id = id;
}
hge::core::Protocol::Types::IdType hge::render::GeometryUnit::getDataId()
{
	return id;
}