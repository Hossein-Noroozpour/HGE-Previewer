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
			enum instructions
			{
				new_object
				/// TODO: Complete this list.
			};
			enum object_type_id
			{
				terrain,
				geometry
				/// TODO: complete this list
			};
			typedef std::uint16_t terrain_aspect_type;
			typedef std::uint8_t  terrain_vbo_components_count_type;
			typedef std::uint32_t id_type;
			typedef std::uint16_t object_type_id_type;
			typedef std::uint16_t instructions_type;
		};
	}
}
#endif
