#pragma once

#include <string>
#include <cstdint>
#include <chrono>
#include <list>

namespace ECAppLogCPP
{

enum class Command : uint8_t
{
	Command_Banner			= 99,
	Command_Log				= 0,
	Command_Quit			= 255
};

class Cmd
{
public:
	Cmd() {}

	virtual Command getCommand() = 0;
	virtual std::string marshalJSON() = 0;
};

class CmdLog : public Cmd
{
public:
	CmdLog(const std::chrono::system_clock::time_point& time, const std::string& priority, const std::string& message,
		const std::string& source, const std::list<std::string>& extraCategories) :
		time(time), priority(priority), message(message), source(source), extraCategories(extraCategories) {}

	std::chrono::system_clock::time_point time;
	std::string priority;
	std::string category;
	std::string message;
	std::string source;
	std::list<std::string> extraCategories;

	std::string marshalJSON();
	Command getCommand() { return Command::Command_Log; }
};

}
