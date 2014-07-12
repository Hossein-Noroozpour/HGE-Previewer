#ifndef HGECLIENTSHAREDMEMORYCONNECTOR_HPP
#define HGECLIENTSHAREDMEMORYCONNECTOR_HPP
#include "hge-client-connector.hpp"
/// TODO: Reimplement this file with boost interprocess
//#include <QtCore/QThread>
//QT_BEGIN_NAMESPACE
//class QSystemSemaphore;
//class QSharedMemory;
//class QMutex;
//QT_END_NAMESPACE
//namespace hge
//{
//	namespace utility
//	{
//		class ClientSharedMemoryConnector : public QThread, public ClientConnector
//		{
//		private:
//			Q_OBJECT
//			QSystemSemaphore *receive_signal;
//			QSystemSemaphore *read_signal;
//			QSharedMemory *channel;
//			const QString channel_key;
//			const QString receive_signal_key;
//			const QString read_signal_key;
//			const quint32 channel_size;
//			core::ApplicationUnit *application;
//			volatile bool in_terminating;
//			QMutex *in_process;
//		public:
//			ClientSharedMemoryConnector(
//					core::ApplicationUnit *const &application,
//					const QString &channel_key = tr("hge_server_channel_01"),
//					/// WARNING for final release make it something unreadable
//					const QString &receive_signal_key = tr("hge_server_receive_signal_01"),
//					/// WARNING for final release make it something unreadable
//					const QString &read_signal_key = tr("hge_server_read_signal_01"),
//					/// WARNING for final release make it something unreadable
//					const quint32 &channel_size = 1024 * 1024 * 32,
//					const bool &fail_on_channel_existance = true,
//					QObject *const &parent = 0);
//			~ClientSharedMemoryConnector();
//			void startConnection();
//			void terminateConnection();
//			void setApplication(core::ApplicationUnit *const &application);
//			core::ApplicationUnit *getApplication();
//		private:
//			void run();
//		};
//	}
//}
#endif
