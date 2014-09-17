#include "hge-mesh-unit.hpp"
#include <iostream>
#define HGEPRINTCODELINE std::cout << "Debugging: file:" << __FILE__ << " line:" << __LINE__ << std::endl << std::flush;
hge::render::MeshUnit::MeshUnit() :
vbo(0), ibo(0), vboSize(0), vboElementsNumber(0), iboSize(0), numberOfIndices(0)
{}
hge::render::MeshUnit::MeshUnit(
	const GLfloat* const& vertices,
	const GLuint* const& indices,
	const GLuint& verticesByteSize,
	const GLuint& verticesNumber,
	const GLuint& indicesByteSize) :
	vboSize(verticesByteSize),
	vboElementsNumber(verticesNumber),
	iboSize(indicesByteSize),
	numberOfIndices((iboSize) / (sizeof(GLuint)))
{
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glGenBuffers(1, &vbo);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glBufferData(GL_ARRAY_BUFFER, verticesByteSize, vertices, GL_STATIC_DRAW);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glGenBuffers(1, &ibo);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesByteSize, indices, GL_STATIC_DRAW);
#ifdef HGEMESHDEBUG
	for(unsigned int i = 0; i < verticesByteSize / sizeof(GLfloat); i++)
		std::cout << i << ":" << vertices[i] << "  ";
	std::cout << "vbo: " << vbo << "  ibo: " << ibo << "  vboSize: " << vboSize
		<< "  vboElementsNumber: " << vboElementsNumber << " iboSize: "
		<< iboSize << " numberOfIndices: " << numberOfIndices << std::endl << std::endl << std::flush;
#endif
	delete[] vertices;
	delete[] indices;
}
hge::render::MeshUnit::~MeshUnit()
{///\todo There is some problem in here, solve it as soon as possible.
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ibo);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif

}
void
hge::render::MeshUnit::bindVBO()
{
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
}
void
hge::render::MeshUnit::bindIBO()
{
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
}
void
hge::render::MeshUnit::bindTotal()
{
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
}
void
hge::render::MeshUnit::draw()
{
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);
#ifdef HGEOPENGLDEBUGMESH
	hge::ui::HGEGLFWWindow::printOpenGLErrorAndTerminate();
#endif
}
GLuint
hge::render::MeshUnit::getVBO()
{
	return vbo;
}
GLuint
hge::render::MeshUnit::getIBO()
{
	return ibo;
}
void hge::render::MeshUnit::setData(std::istream &stream, const core::Protocol::ObjectSizeType &size, const bool &endianCompatible)
{
	core::Protocol::VerticesElementsCountType verticesCount;
	stream.read((char *)(&verticesCount), sizeof verticesCount);
	if (!endianCompatible) swapObject((char *)(&verticesCount), sizeof verticesCount);
	GLfloat *vertices = new GLfloat[verticesCount];
	stream.read((char *)(vertices), sizeof(GLfloat) * verticesCount);
	if (!endianCompatible)
		for (core::Protocol::VerticesElementsCountType i = 0; i < verticesCount; i++)
			swapObject((char *)(&vertices[i]), sizeof GLfloat);
	core::Protocol::IndicesCountType indicesCount;
	stream.read((char *)(&indicesCount), sizeof indicesCount);
	if (!endianCompatible) swapObject((char *)(&indicesCount), sizeof indicesCount);
	GLuint *indices = new GLuint[indicesCount];
	stream.read((char *)(indices), sizeof(GLuint) * indicesCount);
	if (!endianCompatible)
		for (core::Protocol::IndicesCountType i = 0; i < indicesCount; i++)
			swapObject((char *)(&indices[i]), sizeof GLuint);
	if (vbo != 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &vbo);
	}
	if (ibo != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &ibo);
	}
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof GLfloat, vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof GLuint, indices, GL_STATIC_DRAW);
	delete[] vertices;
	delete[] indices;
}