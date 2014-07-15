#ifndef HGETEXTUREUNIT_HPP
#define	HGETEXTUREUNIT_HPP
#include "hge-glfw-window.hpp"
#include <string>
#include <FreeImage.h>
namespace hge
{
	namespace texture
	{
		class TextureUnit
		{
		public:
			TextureUnit(GLenum TextureTarget, const std::string& FileName);
			bool load();
			void bind(GLenum TextureUnit);
		private:
			GLenum m_textureTarget;
			GLuint m_textureObj;
		};
	}
}
#endif	/* TEXTURE_H */
