#include "ECAppLogCPP/Cmd.h"

#include "date.h"

namespace ECAppLogCPP
{

std::string CmdLog::marshalJSON()
{
	std::string timef(date::format("%FT%T", date::floor<std::chrono::milliseconds>(time)));

	return "";
}

}
