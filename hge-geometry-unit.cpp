#include "hge-geometry-unit.hpp"
#include <iostream>
#define HGEPRINTCODELINE std::cout << "Debugging: file:" << __FILE__ << " line:" << __LINE__ << std::endl << std::flush;
hge::render::GeometryUnit::GeometryUnit
(const std::string& id, const std::string& name):
id(id), name(name)
#ifdef HGE_BASIC_QUERY_SUPPORT
,mvp(math::Matrix4D<>(1.0f))
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
void
hge::render::GeometryUnit::setMesh(const std::shared_ptr<hge::render::MeshUnit>& m)
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
void
hge::render::GeometryUnit::occlusionQuery(const math::Matrix4D<> &vp)
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
}
#endif
bool hge::render::GeometryUnit::isThisYourID(const std::string& idstr)
{
	return id == idstr;
}
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
hge::math::ModelUnit*
hge::render::GeometryUnit::getModelMatrix()
{
	return &modelMatrix;
}
std::shared_ptr<hge::render::MeshUnit> hge::render::GeometryUnit::getMesh()
{
	return mesh;
}
#ifdef HGE_BASIC_QUERY_SUPPORT
void
hge::render::GeometryUnit::setOcclusionQueryMesh(const std::shared_ptr<MeshUnit> &m)
{
	occlusionQueryMesh = m;
}
#endif
void hge::render::GeometryUnit::setData(std::istream &stream, const core::Protocol::ObjectSizeType &size, const bool &endianCompatible)
{
	auto beginPos = stream.tellg();
	/// ID
	core::Protocol::StringLengthType idStringLength;
	stream.read((char *)(&idStringLength), sizeof idStringLength);
	if (!endianCompatible) swapObject((char *)(&idStringLength), sizeof idStringLength);
	char *idChars = new char[idStringLength];
	stream.read(idChars, idStringLength);
	id = std::string(idChars, idStringLength);
	delete [] idChars;
	/// Name
	core::Protocol::StringLengthType nameStringLength;
	stream.read((char *)(&nameStringLength), sizeof nameStringLength);
	if (!endianCompatible) swapObject((char *)(&nameStringLength), sizeof nameStringLength);
	char *nameChars = new char[nameStringLength];
	stream.read(nameChars, nameStringLength);
	id = std::string(nameChars, nameStringLength);
	delete[] nameChars;
	/// Mesh
	core::Protocol::IdType meshId;
	stream.read((char *)(&meshId), sizeof meshId);
	//if (!endianCompatible) swapObject((char *)(&meshId), sizeof meshId);
	core::Protocol::ObjectSizeType meshSize;
	stream.read((char *)(&meshSize), sizeof meshSize);
	if (!endianCompatible) swapObject((char *)(&meshSize), sizeof meshSize);
	mesh = std::shared_ptr<MeshUnit>(new MeshUnit());
	mesh->setData(stream, meshSize, endianCompatible);
	if (stream.tellg() - beginPos < size)
	{
		stream.read((char *)(&meshId), sizeof meshId);
		//if (!endianCompatible) swapObject((char *)(&meshId), sizeof meshId);
		stream.read((char *)(&meshSize), sizeof meshSize);
		if (!endianCompatible) swapObject((char *)(&meshSize), sizeof meshSize);
		occlusionQueryMesh = std::shared_ptr<MeshUnit>(new MeshUnit());
		occlusionQueryMesh->setData(stream, meshSize, endianCompatible);
	}
}