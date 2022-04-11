#pragma once

#include <string>
#include <chrono>

namespace ecapplogcpp
{

class Priority
{
public: 
	inline static const std::string Priority_TRACE = "TRACE";
	inline static const std::string Priority_DEBUG = "DEBUG";
	inline static const std::string Priority_INFORMATION = "INFORMATION";
	inline static const std::string Priority_NOTICE = "NOTICE";
	inline static const std::string Priority_WARNING = "WARNING";
	inline static const std::string Priority_FATAL = "FATAL";
	inline static const std::string Priority_CRITICAL = "CRITICAL";
	inline static const std::string Priority_ERROR = "ERROR";
};

class Util
{
public:
	static std::chrono::system_clock::time_point mktime(int year, int mon, int day, int hour, int min, int sec, int msec);
	static std::string fmttime(const std::chrono::system_clock::time_point& time);
};

template<class T>
class TimeProvider
{
public:
	// time format (UTC): YYYY-MM-DDTHH:NN:SS.LLL
	virtual std::string getTime() const = 0;
};

template<>
class TimeProvider<std::chrono::system_clock::time_point>
{
public:
	TimeProvider(const std::chrono::system_clock::time_point &time) : time(time) {}

	std::string getTime() const {
		return Util::fmttime(time);
	}

	std::chrono::system_clock::time_point time;
};

}
