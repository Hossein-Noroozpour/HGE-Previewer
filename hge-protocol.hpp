#ifndef HGEPROTOCOL_HPP
#define HGEPROTOCOL_HPP
#include <cstdint>
namespace hge
{
	namespace core
	{
		class Protocol
		{
		public:
			enum Instructions
			{
				NewObject
				/// TODO: Complete this list.
			};
			enum ObjectTypeId
			{
				Terrain,
				Geometry,
				SkyBox,
				Mesh
				/// TODO: complete this list
			};
			typedef std::uint16_t TerrainAspectType;
			typedef std::uint8_t  TerrainVboComponentsCountType;
			typedef std::uint32_t IdType;
			typedef std::uint16_t ObjectTypeIdType;
			typedef std::uint16_t InstructionsType;
			typedef std::uint32_t ObjectSizeType;
			typedef float VertexElementType;
			typedef std::uint32_t IndexElementType;
			typedef std::uint32_t VerticesElementsCountType;
			typedef std::uint32_t IndicesCountType;
			typedef std::uint16_t StringLengthType;
		};
	}
}
#endif
