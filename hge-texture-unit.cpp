#include "hge-texture-unit.hpp"
#include <iostream>
hge::texture::TextureUnit::TextureUnit(GLenum TextureTarget, const std::string& FileName)
{
	auto bitmap = FreeImage_Load(FreeImage_GetFileType(FileName.c_str(), 0), FileName.c_str());
	m_textureTarget = TextureTarget;
	auto pImage = FreeImage_ConvertTo32Bits(bitmap);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);
	glGenTextures(1, &m_textureObj);
	glBindTexture(m_textureTarget, m_textureObj);
	glTexImage2D(m_textureTarget, 0, GL_RGBA8, nWidth, nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE,
		(void*)FreeImage_GetBits(pImage));
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(m_textureTarget);
	FreeImage_Unload(pImage);
	FreeImage_Unload(bitmap);
}
bool hge::texture::TextureUnit::load()
{
	return true;
}
void hge::texture::TextureUnit::bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}
