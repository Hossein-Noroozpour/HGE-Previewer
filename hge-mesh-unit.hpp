#ifndef HGE_MESH_UNIT_HPP_
#define HGE_MESH_UNIT_HPP_
#ifdef ANDROID
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif
//#define HGEMESHDEBUG
//#define HGEOPENGLDEBUGMESH
namespace hge
{
	namespace render
	{
		class MeshUnit
		{
		public:
			///\warning Do not use this ever, this function is only for
			/// internal use.
			MeshUnit();
			///\warning If you do not use this function properly
			/// it will cause core dump!
			MeshUnit(
					const GLfloat* const& vertices,
					const GLuint* const& indices,
					const GLuint& verticesByteSize,
					const GLuint& verticesNumber,
					const GLuint& indicesByteSize);
			~MeshUnit();
			void bindVBO();
			void bindIBO();
			void bindTotal();
			void draw();
			GLuint getVBO();
			GLuint getIBO();
		private:
			GLuint vbo, ibo, vboSize, vboElementsNumber, iboSize, numberOfIndices;
		};
	}
}
#endif
