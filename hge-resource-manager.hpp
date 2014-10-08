#include "hge-configure.hpp"
#include <memory>
#include "hge-scene-unit.hpp"
namespace hge
{
	namespace core
	{
		class ResourceManager
		{
		public:
			enum exceptions
			{
				CanNotOpenFile,
				ErrorInStream
			};
			static std::shared_ptr<render::SceneUnit> importScene(std::istream &stream);
			static std::shared_ptr<render::SceneUnit> importScene(const std::string &fileAddress);
		private:
			static bool endianCompatibilityCheck(std::istream &stream);
		};
	}
}