#include "hge-texture-unit.hpp"
#include <iostream>
#include <fstream>
#include <png.h>
#define HGE_PNG_SIGNATURE_SIZE 8
static void pngDataReader(png_structp pngPtr, png_bytep data, png_size_t length)
{
	png_voidp a = png_get_io_ptr(pngPtr);
	((std::istream*)a)->read((char*)data, length);
}
void hge::texture::TextureUnit::constructor(std::istream &source)
{
	if (!validate(source))
	{
		throw UnrecognizedFormat;
	}
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (pngPtr == NULL)
	{
		throw InitializationError;
	}
	png_infop infoPtr = png_create_info_struct(pngPtr);
	if (infoPtr == NULL)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp)0, (png_infopp)0);
		throw InformationError;
	}
	png_bytep* rowPtrs = NULL;
	char* data = NULL;
	if (setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)0);
		if (rowPtrs != NULL) delete[] rowPtrs;
		if (data != NULL) delete[] data;
		throw ParsingError;
	}
	png_set_read_fn(pngPtr, (png_voidp)&source, pngDataReader);
	png_set_sig_bytes(pngPtr, HGE_PNG_SIGNATURE_SIZE);
	png_read_info(pngPtr, infoPtr);
	png_uint_32 imgWidth = png_get_image_width(pngPtr, infoPtr);
	png_uint_32 imgHeight = png_get_image_height(pngPtr, infoPtr);
	png_uint_32 bitdepth = png_get_bit_depth(pngPtr, infoPtr);
	png_uint_32 channels = png_get_channels(pngPtr, infoPtr);
	png_uint_32 color_type = png_get_color_type(pngPtr, infoPtr);
#ifdef HGE_TEXTURE_PRINT_INFO
	std::cout << "Image width " << imgHeight << std::endl;
	std::cout << "Image height " << imgWidth << std::endl;
	std::cout << "Image bit depth " << bitdepth << std::endl;
	std::cout << "Image channels " << channels << std::endl;
	std::cout << "Image color type " << color_type << std::endl;
#endif
	switch (color_type)
	{
	case PNG_COLOR_TYPE_GA:
		if (bitdepth < 8)
			png_set_expand_gray_1_2_4_to_8(pngPtr);
		else
			png_set_gray_to_rgb(pngPtr);
		break;
	case PNG_COLOR_TYPE_GRAY:
		if (bitdepth < 8)
			png_set_expand_gray_1_2_4_to_8(pngPtr);
		else
			png_set_gray_to_rgb(pngPtr);
		break;
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(pngPtr);
		break;
	default:
		break;
	}
	if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(pngPtr);
		channels += 1;
	}
	rowPtrs = new png_bytep[imgHeight];
	data = new char[imgWidth * imgHeight * bitdepth * channels / 8];
	const unsigned int stride = imgWidth * bitdepth * channels / 8;
	for (size_t i = 0; i < imgHeight; i++)
	{
		png_uint_32 q = png_uint_32((imgHeight - i - 1) * stride);
		rowPtrs[i] = (png_bytep)data + q;
	}
	png_read_image(pngPtr, rowPtrs);
	glGenTextures(1, &textureObject);
	glBindTexture(textureTarget, textureObject);
	switch (channels)
	{
	case 4:
		glTexImage2D(textureTarget, 0, GL_RGBA8, imgWidth, imgHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)data);
		break;
	case 3:
		glTexImage2D(textureTarget, 0, GL_RGB8, imgWidth, imgHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, (void*)data);
		break;
	default:
		break;
	}
	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(textureTarget);
	delete[](png_bytep)rowPtrs;
	png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)0);
	delete[] data;
}
hge::texture::TextureUnit::TextureUnit(const GLenum &textureTarget, std::istream &source):
textureTarget(textureTarget)
{
	constructor(source);
}
hge::texture::TextureUnit::TextureUnit(const GLenum &textureTarget, const std::string &fileAddress)
{
	std::ifstream fileStream;
	fileStream.open(fileAddress, std::ios_base::binary);
	if (fileStream.is_open())
	{
		constructor(fileStream);
	}
	else
	{
		throw FailedToOpenFile;
	}
}
bool hge::texture::TextureUnit::validate(std::istream &source)
{
	png_byte pngsig[HGE_PNG_SIGNATURE_SIZE];
	int is_png = 0;
	source.read((char*)pngsig, HGE_PNG_SIGNATURE_SIZE);
	if (!source.good()) throw FailedToRead;
	is_png = png_sig_cmp(pngsig, 0, HGE_PNG_SIGNATURE_SIZE);
	return (is_png == 0);
}
void hge::texture::TextureUnit::bind(const GLenum &textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(textureTarget, textureObject);
}
