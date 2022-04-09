#pragma once

#include <ECAppLogCPP/Cmd.h>

#include <string>
#include <cstdint>

namespace ECAppLogCPP
{

class Client
{
public:
	Client(const std::string &appName = "ECAPPLOG-GO", const std::string& address = "127.0.0.1", uint16_t port = 13991);

	void open();
	void close();
private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};

}
