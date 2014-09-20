#include "hge-terrain-unit.hpp"
#include "hge-camera-unit.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <functional>
#ifdef _WIN32
#define M_PI 3.1415926535897932384626433832795
#endif
#define NORMALCALCULATIONDEBUGMODE
//#define NORMALCALCULATIONDEBUGMOD2
#define NUMBEROFBUFFERCOMPONENTS 12
#define NUMBEROFLODS 4
#define HGEPRINTCODELINE std::cout << "Debugging: file:" << __FILE__ << " line:" << __LINE__ << std::endl << std::flush;
#define PRINTVECTOR(v) std::cout << " x: " << v.x << " y: " << v.y << " z: " << v.z << std::endl;
#define NORMALCALCULATIONDEBUGPRINT \
if(tmpv.z < 0.0)\
{\
	std::cerr << "Error in normal calculation.";\
	HGEPRINTCODELINE\
	std::terminate();\
}
#define CHECKLENGTHZERO \
if(glm::length(tmpv1) == 0.0)\
{\
	std::cerr << "Error normal is zero." << std::endl;\
	PRINTVECTOR(tmpv2)\
	PRINTVECTOR(tmpv3)\
	HGEPRINTCODELINE \
	std::terminate();\
}
#define D1toD2(arr, row, col) arr[(row * width) + col]
//#define HGETERRAINLOADINGDEBUGMODE
hge::render::TerrainUnit::TerrainUnit()
{}
hge::render::TerrainUnit::TerrainUnit(const int16_t *const &heights, const int &aspect,
	const double &verticalDegree, const double &horizontalDegree)
{
	GLfloat *vbo = new GLfloat[aspect * aspect * NUMBEROFBUFFERCOMPONENTS];
	calculateNTBs(aspect, verticalDegree, horizontalDegree, vbo, heights);
	GLuint *ibo = new GLuint[(aspect - 1) * (aspect - 1) * 6];
	for (int l = 0, iboIndex = 0; l < NUMBEROFLODS; l++)
	{
		iboIndex = 0;
		for (int i = 0, lStride = 1 << l; i < aspect - lStride; i += lStride)
		{
			for (int j = 0; j < aspect - lStride; j += lStride)
			{
				ibo[iboIndex++] = GLuint((i            * aspect) + j);
				ibo[iboIndex++] = GLuint(((i + lStride) * aspect) + j);
				ibo[iboIndex++] = GLuint(((i + lStride) * aspect) + j + lStride);
				ibo[iboIndex++] = GLuint((i            * aspect) + j);
				ibo[iboIndex++] = GLuint(((i + lStride) * aspect) + j + lStride);
				ibo[iboIndex++] = GLuint((i            * aspect) + j + lStride);
			}
		}
		addIBO(ibo, iboIndex * sizeof(GLuint));
	}
	setVBO(vbo, aspect * aspect * NUMBEROFBUFFERCOMPONENTS * sizeof(GLfloat));
	delete[] ibo;
	delete[] vbo;
}
void hge::render::TerrainUnit::calculateNTBs(const int &aspect, const double &verticalDegree, const double &horizontalDegree, GLfloat *vbo, const int16_t *const &heights)
{
	(void)horizontalDegree;
	const double earthRaidus = 6371000.0;
	const double deltaDegree = 0.004166667;
	/// Vertical delta distance
	const double vDD = (deltaDegree * earthRaidus * M_PI) / 180.0;
	for (int i = 0, vboIndex = 0; i < aspect; i++)
	{
		const double hDD = ((deltaDegree* earthRaidus * M_PI * cos(((verticalDegree - (i * deltaDegree)) * M_PI) / 180.0)) / 180.0);
		for (int j = 0; j < aspect; j++)
		{
			vbo[vboIndex++] = GLfloat((j - (aspect / 2.0f)) * hDD);
			vbo[vboIndex++] = GLfloat(((aspect / 2.0f) - i) * vDD);
			vbo[vboIndex++] = GLfloat(heights[(i * aspect) + j]);
			vboIndex += 9;
		}
	}
	auto getVEC = [&](const int &i, const int &j, const int &ci, const int &cj) -> hge::math::Vector3D < >
	{
		uint64_t tmpInt1 = (((i*aspect) + j)*NUMBEROFBUFFERCOMPONENTS);
		uint64_t tmpInt2 = (((ci*aspect) + cj)*NUMBEROFBUFFERCOMPONENTS);
		return math::Vector3D<>(vbo[tmpInt1] - vbo[tmpInt2],
			vbo[tmpInt1 + 1] - vbo[tmpInt2 + 1],
			vbo[tmpInt1 + 2] - vbo[tmpInt2 + 2]);
	};
	int i, j, vboIndex;
	for (i = 0, vboIndex = 0; i < aspect; i++)
	{
		for (j = 0; j < aspect; j++)
		{
			math::Vector3D<> nv(0.0f, 0.0f, 0.0f);
			if (i > 0)
			{
				if (j > 0)
				{
					math::Vector3D<> N = getVEC(i - 1, j, i, j);
					math::Vector3D<> W = getVEC(i, j - 1, i, j);
					math::Vector3D<> NW = getVEC(i - 1, j - 1, i, j);
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(N, NW));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(NW, W));
				}
				if (j < aspect - 1)
				{
					math::Vector3D<> N = getVEC(i - 1, j, i, j);
					math::Vector3D<> NE = getVEC(i - 1, j + 1, i, j);
					math::Vector3D<> E = getVEC(i, j + 1, i, j);
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(NE, N));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(E, NE));
				}
			}
			if (i < aspect - 1)
			{
				if (j > 0)
				{
					math::Vector3D<> S = getVEC(i + 1, j, i, j);
					math::Vector3D<> SW = getVEC(i + 1, j - 1, i, j);
					math::Vector3D<> W = getVEC(i, j - 1, i, j);
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(SW, S));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(W, SW));
				}
				if (j < aspect - 1)
				{
					math::Vector3D<> S = getVEC(i + 1, j, i, j);
					math::Vector3D<> E = getVEC(i, j + 1, i, j);
					math::Vector3D<> SE = getVEC(i + 1, j + 1, i, j);
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(S, SE));
					nv += math::Vector3D<>::normalize(math::Vector3D<>::cross(SE, E));
				}
			}
			nv = math::Vector3D<>::normalize(nv);
			math::Vector3D<> tv = math::Vector3D<>::normalize(math::Vector3D<>(0.0f, 1.0f, 0.0f) - (nv * (nv.vec[1])));
			math::Vector3D<> btv = math::Vector3D<>::cross(nv, tv);
			vboIndex += 3;
			vbo[vboIndex++] = nv.vec[0];
			vbo[vboIndex++] = nv.vec[1];
			vbo[vboIndex++] = nv.vec[2];
			vbo[vboIndex++] = tv.vec[0];
			vbo[vboIndex++] = tv.vec[1];
			vbo[vboIndex++] = tv.vec[2];
			vbo[vboIndex++] = btv.vec[0];
			vbo[vboIndex++] = btv.vec[1];
			vbo[vboIndex++] = btv.vec[2];
		}
	}
}
hge::render::TerrainUnit::~TerrainUnit()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vboBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	for (unsigned int i = 0; i < iboBuffers.size(); i++)
	{
		glDeleteBuffers(1, &(iboBuffers[i]));
	}
}
void hge::render::TerrainUnit::draw(const math::Matrix4D<> &vp)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboBuffer);
	shader->use();
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		textures[i]->bind(GL_TEXTURE0 + i);
	}
	shader->setModelViewProjectionMatrix(vp * modelMatrix.getConstRotateScaleTranslateMatrix());
	for (unsigned int i = 0; i < iboBuffers.size(); i++)
	{
		shader->setLODNumber(i);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboBuffers[i]);
		glDrawElements(GL_TRIANGLES, iboElements[i], GL_UNSIGNED_INT, (void *)(0));
	}
}
void hge::render::TerrainUnit::setVBO(const GLfloat * const &vertices, const GLuint &verticesBytes)
{
	glGenBuffers(1, &vboBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vboBuffer);
	glBufferData(GL_ARRAY_BUFFER, verticesBytes, vertices, GL_STATIC_DRAW);
}
void hge::render::TerrainUnit::addIBO(const GLuint * const &indices, const GLuint &indicesBytes)
{
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesBytes, indices, GL_STATIC_DRAW);
	iboElements.push_back(indicesBytes / (sizeof(GLuint)));
	iboBuffers.push_back(ibo);
}
void hge::render::TerrainUnit::setShader(const std::shared_ptr<shader::ShaderUnit>& s)
{
	shader = s;
}
void hge::render::TerrainUnit::addTexture(const std::shared_ptr<texture::TextureUnit>& texture)
{
	textures.push_back(texture);
}
hge::math::ModelUnit *hge::render::TerrainUnit::getModelMatrix()
{
	return &modelMatrix;
}
void hge::render::TerrainUnit::setData(std::istream &stream, const bool &endianCompatible)
{
#define HGE_READ_BTYPE(t) stream.read((char *)(&t), sizeof t);if (!endianCompatible) swapObject((char *)(&t), sizeof t)
#define HGE_TEST_FORMAT if (stream.eof()) { std::cerr << __FILE__ << ": Format error!" << std::endl;}
	core::Protocol::Types::TerrainAspectType aspect;
	HGE_READ_BTYPE(aspect);
#ifdef HGE_TEST_MODE
	std::cout << __FILE__ << "(" << __LINE__ << "): aspect is: " << aspect << std::endl;
#endif
#ifdef HGE_INTENSIVE_DEBUG_MODE
	if (std::powf(2.f, std::floorf(std::log2f(aspect))) != float(aspect))
	{
		std::cerr << __FILE__ << ": Error: Terrain aspect is not a power of 2 number." << std::endl;
		std::terminate();
	}
	HGE_TEST_FORMAT;
#endif
	HGE_READ_BTYPE(id);
#ifdef HGE_INTENSIVE_DEBUG_MODE
	if (id < 8)
	{
		std::cerr << __FILE__ << ": Error: Terrain ID is less than 8 this means its exporter part doesn't work very well." << std::endl;
	}
	HGE_TEST_FORMAT;
#endif
	core::Protocol::Types::TerrainVboComponentsCountType vbo_com_cnt;
	HGE_READ_BTYPE(vbo_com_cnt);
#ifdef HGE_DEBUG_MODE
	if (HGE_TERRAIN_COMPONENT_COUNT != vbo_com_cnt)
	{
		/// \todo Add support for different VBO component count.
		std::cerr << __FILE__ << ": Error: Terrain VBO component is not equal to imported terrain." << std::endl;
		std::terminate();
	}
	HGE_TEST_FORMAT;
#endif
	core::Protocol::Types::VertexElementType *vbo_data = new core::Protocol::Types::VertexElementType[vbo_com_cnt * aspect * aspect];
	stream.read((char *)(vbo_data), aspect * aspect * vbo_com_cnt * sizeof(core::Protocol::Types::VertexElementType));
	if (!endianCompatible)
	{
		for (int i = 0; i < aspect * aspect * vbo_com_cnt; i++)
		{
			swapObject((char *)(&(vbo_data[i])), sizeof(core::Protocol::Types::VertexElementType));
		}
	}
	GLuint *ibo = new GLuint[(aspect - 1) * (aspect - 1) * 6];
	for (int l = 0, iboIndex = 0; l < NUMBEROFLODS; l++)
	{
		iboIndex = 0;
		for (int i = 0, lStride = 1 << l; i < aspect - lStride; i += lStride)
		{
			for (int j = 0; j < aspect - lStride; j += lStride)
			{
				ibo[iboIndex++] = GLuint((i * aspect) + j);
				ibo[iboIndex++] = GLuint(((i + lStride) * aspect) + j);
				ibo[iboIndex++] = GLuint(((i + lStride) * aspect) + j + lStride);
				ibo[iboIndex++] = GLuint((i * aspect) + j);
				ibo[iboIndex++] = GLuint(((i + lStride) * aspect) + j + lStride);
				ibo[iboIndex++] = GLuint((i * aspect) + j + lStride);
			}
		}
		addIBO(ibo, iboIndex * sizeof(GLuint));
	}
	setVBO(vbo_data, aspect * aspect * NUMBEROFBUFFERCOMPONENTS * sizeof(core::Protocol::Types::VertexElementType));
	delete[] ibo;
	delete[] vbo_data;
}
void hge::render::TerrainUnit::setDataId(const core::Protocol::Types::IdType &id)
{
	this->id = id;
}
hge::core::Protocol::Types::IdType hge::render::TerrainUnit::getDataId()
{
	return id;
}