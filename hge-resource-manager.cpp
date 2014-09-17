//#include <stdio.h>
//#include <zlib.h>
//unsigned long file_size(char *filename)
//{
//	FILE *pFile = fopen(filename, "rb");
//	fseek(pFile, 0, SEEK_END);
//	unsigned long size = ftell(pFile);
//	fclose(pFile);
//	return size;
//}
//int decompress_one_file(char *infilename, char *outfilename)
//{
//	gzFile infile = gzopen(infilename, "rb");
//	FILE *outfile = fopen(outfilename, "wb");
//	if (!infile || !outfile) return -1;
//	char buffer[128];
//	int num_read = 0;
//	while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) 
//	{
//		fwrite(buffer, 1, num_read, outfile);
//	}
//	gzclose(infile);
//	fclose(outfile);
//}
//int compress_one_file(char *infilename, char *outfilename)
//{
//	FILE *infile = fopen(infilename, "rb");
//	gzFile outfile = gzopen(outfilename, "wb");
//	if (!infile || !outfile) return -1;
//	char inbuffer[128];
//	int num_read = 0;
//	unsigned long total_read = 0, total_wrote = 0;
//	while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {
//		total_read += num_read;
//		gzwrite(outfile, inbuffer, num_read);
//	}
//	fclose(infile);
//	gzclose(outfile);
//	printf("Read %ld bytes, Wrote %ld bytes, Compression factor %4.2f%%\n",
//		total_read, file_size(outfilename),
//		(1.0 - file_size(outfilename)*1.0 / total_read)*100.0);
//}
#include "hge-resource-manager.hpp"
#include <fstream>
std::shared_ptr<hge::render::SceneUnit> hge::core::ResourceManager::importScene(std::istream &stream)
{
	bool endianCompatible = endianCompatibilityCheck(stream);
	std::shared_ptr<render::SceneUnit> result(new render::SceneUnit());
	Protocol::ObjectSizeType dataSize;
	Protocol::IdType id;
	Protocol::ObjectTypeIdType typeId;
	for(
		stream.read((char *)(&id), sizeof id),
		stream.read((char *)(&typeId), sizeof typeId),
		stream.read((char *)(&dataSize), sizeof dataSize)
		;
	!stream.eof()
		;
	stream.read((char *)(&id), sizeof id),
		stream.read((char *)(&typeId), sizeof typeId),
		stream.read((char *)(&dataSize), sizeof dataSize))
	{
		switch (typeId)
		{
		case Protocol::Geometry:
		{
			std::shared_ptr<render::GeometryUnit> geo(new render::GeometryUnit("", ""));
			result->addGeometry(geo);
			break;
		}
		case Protocol::Terrain:
			break;
		case Protocol::Mesh:
			break;
		case Protocol::SkyBox:
			break;
		default:
			break;
		}
	}
	return result;
}
std::shared_ptr<hge::render::SceneUnit> hge::core::ResourceManager::importScene(const std::string &fileAddress)
{
#ifdef ANDROID
#else
	std::ifstream file;
	file.open(fileAddress, std::ios::binary | std::ios::in);
	if (!file.is_open()) throw CanNotOpenFile;
	std::shared_ptr<hge::render::SceneUnit> result = importScene(file);
	file.close();
	return result;
#endif
}
bool hge::core::ResourceManager::endianCompatibilityCheck(std::istream &stream)
{
	int systemEndian = 1;
	char resourceEndian;
	stream.read(&resourceEndian, 1);
	return resourceEndian == ((char *)(&systemEndian))[0];
}