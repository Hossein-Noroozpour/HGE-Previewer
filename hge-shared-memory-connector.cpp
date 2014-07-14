//#include "hge-client-shared-memory-connector.hpp"
//#include "hge-application-unit.hpp"
//#include <QSystemSemaphore>
//#include <QSharedMemory>
//#include <QMessageBox>
//#include <QMutex>
//#include <QThread>
//#include <QDebug>
//#define HGEDEBUG qDebug() << __LINE__ << __FILE__;
//hge::utility::ClientSharedMemoryConnector::ClientSharedMemoryConnector(
//		core::ApplicationUnit *const &application,
//		const QString &channel_key,
//		const QString &receive_signal_key,
//		const QString &read_signal_key,
//		const quint32 &channel_size,
//		const bool &fail_on_channel_existance,
//		QObject *const &parent) :
//	QThread(parent),
//	receive_signal(new QSystemSemaphore(receive_signal_key, 0, QSystemSemaphore::Create)),
//	read_signal(new QSystemSemaphore(read_signal_key, 0, QSystemSemaphore::Create)),
//	channel(new QSharedMemory(channel_key)),
//	channel_key(channel_key),
//	receive_signal_key(receive_signal_key),
//	read_signal_key(read_signal_key),
//	channel_size(channel_size),
//	application(application),
//	in_terminating(false),
//	in_process(new QMutex())
//{
//	setTerminationEnabled();
//	/// TODO severe error checking
//	if(fail_on_channel_existance)
//	{
//		if(!channel->create(channel_size))
//		{
//			qDebug() << channel->errorString();
//			delete channel;
//			delete receive_signal;
//			delete read_signal;
//			throw channel_already_exist;
//		}
//	}
//	else
//	{
//		if(!channel->create(channel_size))
//		{
//			qDebug() << channel->errorString();
//			channel->attach();
//		}
//	}
//}
//hge::utility::ClientSharedMemoryConnector::~ClientSharedMemoryConnector()
//{
//	delete channel;
//	delete receive_signal;
//	delete read_signal;
//}
//void hge::utility::ClientSharedMemoryConnector::startConnection()
//{
//	this->start();
//}
//void hge::utility::ClientSharedMemoryConnector::setApplication(core::ApplicationUnit *const &application)
//{
//	this->application = application;
//}
//hge::core::ApplicationUnit *hge::utility::ClientSharedMemoryConnector::getApplication()
//{
//	return application;
//}
//void hge::utility::ClientSharedMemoryConnector::run()
//{
//	while(true)
//	{
//		receive_signal->acquire();
//		if(in_terminating)
//		{
//			return;
//		}
//		HGEDEBUG
//		in_process->lock();
//		channel->lock();
//		HGEDEBUG
//		const unsigned int data_size = *((int *)(channel->data()));
//		const unsigned int data_real_size = data_size + sizeof(unsigned int);
//		unsigned char *data = new unsigned char[data_size];
//		HGEDEBUG
//		memcpy(data, (void *)((unsigned long)channel->data() + sizeof(unsigned int)),
//			   qMin((int)(data_real_size), (int)(channel_size)) - sizeof(unsigned int));
//		channel->unlock();
//		read_signal->release();
//		HGEDEBUG
//		for(unsigned int i = 1; i < data_real_size / channel_size; i++)
//		{
//			HGEDEBUG
//			receive_signal->acquire();
//			channel->lock();
//			memcpy(data + (i * channel_size) - sizeof(unsigned int), channel->data(), channel_size);
//			if((data_real_size / channel_size) * channel_size == data_real_size)
//			{
//				application->newData(data_size, data);
//			}
//			channel->unlock();
//			read_signal->release();
//		}
//		HGEDEBUG
//		if((data_real_size / channel_size) * channel_size != data_real_size)
//		{
//			receive_signal->acquire();
//			channel->lock();
//			memcpy(data + ((data_real_size / channel_size) * channel_size) - sizeof(unsigned int),
//				   channel->data(), data_real_size - ((data_real_size / channel_size) * channel_size));
//			application->newData(data_size, data);
//			channel->unlock();
//			read_signal->release();
//		}
//		in_process->unlock();
//		qDebug() << data_size;
//	}
//}
//void hge::utility::ClientSharedMemoryConnector::terminateConnection()
//{
//	in_terminating = true;
//	in_process->lock();
//	receive_signal->release(10);
//	this->wait();
//}
