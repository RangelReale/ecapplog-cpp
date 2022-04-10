#include "Cmd.h"

#include "date.h"
#include <nlohmann/json.hpp>

namespace ecapplogcpp
{

std::string CmdLog::marshalJSON()
{
	std::string timef(date::format("%FT%T", date::floor<std::chrono::milliseconds>(time)));

	nlohmann::json j;
	j["time"] = timef;
	j["priority"] = priority;
	j["category"] = category;
	j["message"] = message;
	if (!source.empty()) {
		j["source"] = source;
	}
	if (!extraCategories.empty()) {
		j["extra_categories"] = extraCategories;
	}

	return j.dump();
}

}
