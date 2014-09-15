#ifndef HGETEXTUREUNIT_HPP
#define	HGETEXTUREUNIT_HPP
#ifdef ANDROID
#ifdef HGE_USE_OPENGL_ES_3
#inlude <GLES/gl3.h>
#else
#include <GLES2/gl2.h>
#endif
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif
#include <istream>
namespace hge
{
	namespace texture
	{
		class TextureUnit
		{
		public:
			enum exceptions
			{
				FailedToRead,
				UnrecognizedFormat,
				InitializationError,
				InformationError,
				ParsingError
			};
			TextureUnit(const GLenum &TextureTarget, std::istream &source);
			bool validate(std::istream &source);
			void bind(const GLenum &TextureUnit);
		private:
			GLenum textureTarget;
			GLuint textureObject;
		};
	}
}
#endif
