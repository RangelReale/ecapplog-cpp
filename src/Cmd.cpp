#include "Cmd.h"

#include "date.h"
#include <nlohmann/json.hpp>

namespace ecapplogcpp
{

std::string CmdLog::marshalJSON()
{
	nlohmann::json j;
	j["time"] = timeStr;
	j["priority"] = priority;
	j["category"] = category;
	j["message"] = message;
	if (!source.empty()) {
		j["source"] = source;
	}
	if (!color.empty()) {
		j["color"] = color;
	}
	if (!bgcolor.empty()) {
		j["bgcolor"] = bgcolor;
	}
	if (!extraCategories.empty()) {
		j["extra_categories"] = extraCategories;
	}
	return j.dump();
}

}
