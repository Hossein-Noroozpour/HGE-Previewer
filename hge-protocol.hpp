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
			enum instructions : std::uint16_t
			{
				new_object
				/// TODO: Complete this list.
			};
			enum type_id : std::uint16_t
			{
				terrain,
				geometry
				/// TODO: complete this list
			};
			typedef std::uint16_t terrain_aspect_type;
			typedef std::uint8_t terrain_vbo_components_count_type;
			Protocol();
		};
	}
}
#endif
