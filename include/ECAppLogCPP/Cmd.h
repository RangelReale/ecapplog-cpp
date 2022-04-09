#pragma once

#include <string>
#include <cstdint>
#include <chrono>
#include <deque>

namespace ECAppLogCPP
{

enum class Command : uint8_t
{
	Command_Banner			= 99,
	command_Log				= 0
};

class Cmd
{
public:
	Cmd() {}

	virtual std::string marshalJSON() = 0;
};

class CmdLog : public Cmd
{
public:
	std::chrono::system_clock::time_point time;
	std::string priority;
	std::string category;
	std::string message;
	std::string source;
	std::deque<std::string> extraCategories;

	std::string marshalJSON();
};


}
