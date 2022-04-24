#pragma once

#include "ecapplogcpp/Data.h"

#include <string>
#include <cstdint>
#include <chrono>
#include <list>

namespace ecapplogcpp
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
	CmdLog(const std::string& timeStr, const std::string& priority, const std::string& category,
		const std::string& message, const std::string& source, const std::string& color, const std::string& bgcolor, 
		const std::list<std::string>& extraCategories) :
		timeStr(timeStr), priority(priority), category(category), message(message), source(source),
		color(color), bgcolor(bgcolor), extraCategories(extraCategories) {}

	std::string timeStr;
	std::string priority;
	std::string category;
	std::string message;
	std::string source;
	std::string color;
	std::string bgcolor;
	std::list<std::string> extraCategories;

	std::string marshalJSON();
	Command getCommand() { return Command::Command_Log; }
};

}
