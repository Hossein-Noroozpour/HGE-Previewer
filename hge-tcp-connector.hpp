#ifndef HGE_UTILITY_HGECLIENTTCPCONNECTOR_HPP
#define HGE_UTILITY_HGECLIENTTCPCONNECTOR_HPP
#include "hge-connector.hpp"
namespace std
{
	class thread;
}
namespace hge
{
	namespace utility
	{
		class TCPConnector : public Connector
		{
		private:
			core::ApplicationUnit *application;
			std::thread *listenerThread;
			volatile bool terminated;
			void listener();
		public:
			TCPConnector();
			~TCPConnector();
			void startConnection();
			void terminateConnection();
			void setApplication(core::ApplicationUnit *const &application);
			core::ApplicationUnit *getApplication();
			void sendData(const unsigned int &size, const unsigned char *const &data);
		};
	} // namespace utility
} // namespace hge
#endif // HGE_UTILITY_HGECLIENTTCPCONNECTOR_HPP
