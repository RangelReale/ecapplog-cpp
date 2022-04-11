#include "ecapplogcpp/Data.h"

#include "date.h"

namespace ecapplogcpp
{

std::chrono::system_clock::time_point Util::mktime(int year, int mon, int day, int hour, int min, int sec, int msec)
{
	return date::sys_days(date::year_month_day(date::year(year), date::month(mon), date::day(day))) +
		std::chrono::hours{ hour } + std::chrono::minutes{ min } + std::chrono::seconds{ sec } + std::chrono::milliseconds(msec);
}

std::string Util::fmttime(const std::chrono::system_clock::time_point& time)
{
	return date::format("%FT%T", date::floor<std::chrono::milliseconds>(time));
}

}
