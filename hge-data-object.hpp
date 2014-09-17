#ifndef HGEDATAOBJECT_HPP
#define HGEDATAOBJECT_HPP
#include "hge-serializable.hpp"
namespace hge
{
	namespace core
	{
		class DataObject : public Serializable
		{
		public:
			virtual hge::core::Protocol::ObjectTypeIdType getTypeId() = 0;
			virtual void setDataId(const hge::core::Protocol::IdType &id) = 0;
			virtual hge::core::Protocol::IdType getDataId() = 0;
		};
	}
}
#endif
