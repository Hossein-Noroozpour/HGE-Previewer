#ifndef HGE_SHADER_UNIT_HPP_
#define HGE_SHADER_UNIT_HPP_
#ifdef ANDROID
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif
#include "hge-math.hpp"
namespace hge
{
	namespace shader
	{
		class ShaderUnit
		{
		public:
			virtual void setModelMatrix(const math::Matrix4D<> &modelMatrix) = 0;
			virtual void setModelViewProjectionMatrix(const math::Matrix4D<> &modelViewProjectionMatrix) = 0;
			virtual void setLODNumber(const GLuint& lodNumber) = 0;
			virtual void use() = 0;
			virtual GLuint getModelViewProjectionMatrixUniformLocation() = 0;
			virtual GLuint getModelMatrixUniformLoaction() = 0;
			virtual GLuint getVertexShaderProgram() = 0;
			virtual GLuint getGeometryShaderProgram() = 0;
			virtual GLuint getFragmentShaderProgram() = 0;
			virtual GLuint getShaderProgram() = 0;
		};
	}
}
#endif
