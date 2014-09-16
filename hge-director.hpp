#ifndef HGEDIRECTOR_HPP
#define HGEDIRECTOR_HPP
#include "hge-application-unit.hpp"
#include "hge-scene-unit.hpp"
#include <functional>
#include <mutex>
namespace hge
{
	namespace core
	{
		class Director : public ApplicationUnit
		{
		private:
			std::mutex gldo_lock;
			std::vector<std::function<void(void)>> gldo;
			std::shared_ptr<hge::render::SceneUnit> scene;
			bool goForward;
			bool goDownward;
			bool goRightward;
			bool goLeftward;
			bool rotateOn;
			bool wireframeMode;
			float cameraRotationSpeed;
			float cameraMoveSpeed;
		public:
			Director();
			~Director();
			void start();
			void initialize();
			void update();
			void buttonPressed(const HGEButton& key);
			void buttonReleased(const HGEButton& key);
			void mouseMoved(const float &dx, const float &dy);
		};
	}
}
#endif
