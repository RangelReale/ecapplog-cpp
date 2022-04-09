#include "ECAppLogCPP/Client.h"
#include "ECAppLogCPP/SafeQueue.h"

#include "sockpp/tcp_connector.h"

#include <thread>
#include <chrono>

namespace ECAppLogCPP
{

class CmdQuit : public Cmd
{
public:
	std::string marshalJSON() { return ""; }
};


class Client::Impl
{
public:
	Client::Impl(const std::string& appName, const std::string& address, uint16_t port) :
		_appName(appName), _address(address), _port(port), _isOpen(false), _queue()
	{

	}


	void open()
	{
		if (!_isOpen)
		{
			_thread = std::thread(&Client::Impl::handleConnection, this);
			_isOpen = true;
		}
	}

	void close()
	{
		if (_isOpen)
		{
			_isOpen = false;
		}
	}

private:
	void handleConnection()
	{
		sockpp::socket_initializer sockInit;
		while (true)
		{
			try
			{
				handleConnectionLoop();
			}
			catch (std::exception&)
			{

			}
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}

	void handleConnectionLoop()
	{
		sockpp::tcp_connector conn({ _address, 8080 });
		if (!conn) return;
		conn.read_timeout(std::chrono::seconds(5));
		handleBanner(conn);
	}

	void handleBanner(sockpp::tcp_connector& conn)
	{
		// write command
		uint8_t cmd(static_cast<uint8_t>(Command::Command_Banner));
		conn.write(&cmd, sizeof(cmd));

		std::string data("ECAPPLOG ");
		data.append(_appName);

		// write size
		int32_t size = htonl(static_cast<int32_t>(data.size()));
		conn.write(&size, sizeof(size));

		// write data
		conn.write(data);
	}


	std::string _appName, _address;
	uint16_t _port;
	bool _isOpen;
	SafeQueue<std::shared_ptr<Cmd> > _queue;
	std::thread _thread;
};

Client::Client(const std::string& appName, const std::string& address, uint16_t port) :
	_appName(appName), _address(address), _port(port), _isOpen(false), _queue()
{
	
}

void Client::open()
{
	if (!_isOpen)
	{
		_thread = std::thread(&Client::handleConnection, this);
		_isOpen = true;
	}
}

void Client::close()
{
	if (_isOpen)
	{
		_isOpen = false;
	}
}

void Client::handleConnection()
{
	sockpp::socket_initializer sockInit;

	while (true)
	{
		sockpp::tcp_connector conn({ _address, 8080 });
		if (!conn) {
			cerr << "Error connecting to server at "
				<< sockpp::inet_address(host, port)
				<< "\n\t" << conn.last_error_str() << endl;
			return 1;
		}
	}
}

}