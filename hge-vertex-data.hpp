#ifndef HGEVERTEXDATA_HPP
#define	HGEVERTEXDATA_HPP
#ifdef ANDROID
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif
#include <vector>
namespace hge
{
	namespace core
	{
		class VertexData
		{
		public:
			GLfloat px;
			GLfloat py;
			GLfloat pz;
			GLfloat nx;
			GLfloat ny;
			GLfloat nz;
			GLfloat ux;
			GLfloat uy;
			std::vector<unsigned int> indices;
			void print(void);
			bool isEqual(const VertexData& v);
			class Compare
			{
			public:
				bool operator()(const VertexData& v1, const VertexData& v2);
			};
		};
	}
}
#endif
