#include "hge-tcp-connector.hpp"
#include <cstdint>
#include <functional>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include <boost/array.hpp>
hge::utility::TCPConnector::TCPConnector():
	terminated(false)
{
}
void hge::utility::TCPConnector::startConnection()
{
	listenerThread = new std::thread(std::bind(&hge::utility::TCPConnector::listener, this));
}
void hge::utility::TCPConnector::terminateConnection()
{
	if(!terminated)
	{
		terminated = true;
		listenerThread->join();
		delete listenerThread;
	}
}
void hge::utility::TCPConnector::setApplication(core::ApplicationUnit *const &application)
{
	this->application = application;
}
hge::core::ApplicationUnit *hge::utility::TCPConnector::getApplication()
{
	return application;
}
void hge::utility::TCPConnector::listener()
{
	try
	{
		boost::asio::io_service io_service;
		boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), 6666));
		boost::asio::ip::tcp::socket socket(io_service);
		acceptor.accept(socket);
		for (;;)
		{
			boost::system::error_code error;
			std::uint32_t dataSize = 0;
			size_t len = socket.read_some(boost::asio::buffer(&dataSize, sizeof(std::uint32_t)), error);
			if(terminated)
			{
				return;
			}
			if(error != boost::asio::error::eof)
			{
				std::cerr << error.message() << std::endl;
				std::cerr << "Error in file:" << __FILE__ << "  line:" << __LINE__ << std::endl;
				return;
			}
			if(len != sizeof(std::uint32_t))
			{
				std::cerr << "Data length is not as expected." << std::endl;
				std::cerr << "Error in file:" << __FILE__ << "  line:" << __LINE__ << std::endl;
				return;
			}
			if(dataSize == 0)
			{
				std::cerr << "Incorrect message." << std::endl;
				std::cerr << "Error in file:" << __FILE__ << "  line:" << __LINE__ << std::endl;
				return;
			}
			unsigned char *buf = new unsigned char[dataSize];
			len = socket.read_some(boost::asio::buffer(buf, dataSize), error);
			if(terminated)
			{
				return;
			}
			if(error != boost::asio::error::eof)
			{
				std::cerr << error.message() << std::endl;
				std::cerr << "Error in file:" << __FILE__ << "  line:" << __LINE__ << std::endl;
				return;
			}
			if(len != dataSize)
			{
				std::cerr << error.message() << std::endl;
				std::cerr << "Error in file:" << __FILE__ << "  line:" << __LINE__ << std::endl;
				return;
			}
		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
void hge::utility::TCPConnector::sendData(const unsigned int &size, const unsigned char * const &data)
{
	try
	{
		boost::asio::io_service io_service;
		boost::asio::ip::tcp::socket socket(io_service);
		socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), 6667));
//		for (;;)
//		{
//			boost::array<char, 2> buf;
//			boost::system::error_code error;
//			size_t len = socket.read_some(boost::asio::buffer(buf), error);
//			if (error == boost::asio::error::eof)
//			break; // Connection closed cleanly by peer.
//			else if (error)
//			throw boost::system::system_error(error); // Some other error.
//			std::cout.write(buf.data(), len);
//		}
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
