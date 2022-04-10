#include "ecapplogcpp/Client.h"
#include "SafeQueue.h"
#include "Cmd.h"

#include "sockpp/tcp_connector.h"

#include <thread>
#include <chrono>
#include <sstream>

namespace ecapplogcpp
{

class CmdQuit : public Cmd
{
public:
	std::string marshalJSON() { return ""; }
	Command getCommand() { return Command::Command_Quit; }
};

class ConnectionError : public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class Client::Impl
{
public:
	Client::Impl(const std::string& appName, const std::string& address, uint16_t port) :
		_appName(appName), _address(address), _port(port), _isOpen(false), _isRunning(false), _queue()
	{

	}

	void open()
	{
		if (!_isOpen)
		{
			_queue.clear();
			_thread = std::thread(&Client::Impl::handleConnection, this);
			_isOpen = true;
		}
	}

	void close()
	{
		if (_isOpen)
		{
			_isRunning = false;
			_queue.reenqueue(std::make_shared<CmdQuit>()); // add at top
			_thread.join();
			_isOpen = false;
		}
	}

	void log(const std::chrono::system_clock::time_point& time, const std::string& priority, const std::string& category, 
		const std::string& message, const std::string& source, const std::list<std::string>& extraCategories)
	{
		_queue.enqueue(std::make_shared<CmdLog>(time, priority, category, message, source, extraCategories));
	}

private:
	void handleConnection()
	{
		_isRunning = true;

		sockpp::socket_initializer sockInit;
		bool cont = true;
		while (cont && _isRunning)
		{
			cont = true;
			try
			{
				cont = handleConnectionLoop();
			}
			catch (std::exception&)
			{
				// ignore
			}
			if (cont && _isRunning)
				std::this_thread::sleep_for(std::chrono::seconds(5));
		}
	}

	bool handleConnectionLoop()
	{
		sockpp::tcp_connector conn({ _address, _port });
		if (!conn.connect({ _address, _port })) {
			std::stringstream err;
			err << "Error connecting to server at "
				<< sockpp::inet_address(_address, _port)
				<< "\n\t" << conn.last_error_str();
			throw ConnectionError(err.str());
		}
		conn.read_timeout(std::chrono::seconds(5));
		handleBanner(conn);

		while (true)
		{
			auto cmd = _queue.dequeue();
			if (cmd)
			{
				try {
					auto quit = std::dynamic_pointer_cast<CmdQuit>(cmd);
					if (quit) {
						return false;
					}
					handleCmd(conn, cmd);
				}
				catch (ConnectionError &)
				{
					_queue.reenqueue(cmd);
					throw;
				}
			}
		}
		return true;
	}

	void handleBanner(sockpp::tcp_connector& conn)
	{
		std::string data("ECAPPLOG ");
		data.append(_appName);
		writeCmd(conn, Command::Command_Banner, data);
	}

	void handleCmd(sockpp::tcp_connector& conn, std::shared_ptr<Cmd> cmd)
	{
		std::string data = cmd->marshalJSON();
		writeCmd(conn, cmd->getCommand(), data);
	}

	void writeCmd(sockpp::tcp_connector& conn, Command command, const std::string &data)
	{
		// write command
		uint8_t ccmd = static_cast<uint8_t>(command);
		connWrite(conn, &ccmd, sizeof(command));

		// write size
		uint32_t size = htonl(static_cast<uint32_t>(data.size()));
		connWrite(conn, &size, sizeof(size));

		// write data
		connWrite(conn, data.data(), data.size());
	}

	void connWrite(sockpp::tcp_connector& conn, const void* buf, size_t n)
	{
		if (conn.write(buf, n) != n)
		{
			if (conn.last_error() == EPIPE) {
				throw ConnectionError("It appears that the socket was closed.");
			}
			else {
				std::stringstream err;
				err << "Error writing to the TCP stream ["
					<< conn.last_error() << "]: "
					<< conn.last_error_str();
				throw ConnectionError(err.str());
			}
		}
	}

	std::string _appName, _address;
	uint16_t _port;
	bool _isOpen, _isRunning;
	SafeQueue<std::shared_ptr<Cmd> > _queue;
	std::thread _thread;
};

Client::Client(const std::string& appName, const std::string& address, uint16_t port) :
	_impl(new Client::Impl(appName, address, port))
{
	
}

Client::~Client()
{
	close();
}

void Client::open()
{
	_impl->open();
}

void Client::close()
{
	_impl->close();
}

void Client::log(const std::chrono::system_clock::time_point& time, const std::string& priority, const std::string& category, 
	const std::string& message, const std::string& source, const std::list<std::string>& extraCategories)
{
	_impl->log(time, priority, category, message, source, extraCategories);
}

void Client::logNow(const std::string& priority, const std::string& category, const std::string& message,
	const std::string& source, const std::list<std::string>& extraCategories)
{
	log(std::chrono::system_clock::now(), priority, category, message, source, extraCategories);
}

}