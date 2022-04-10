#pragma once

#include <string>
#include <cstdint>
#include <chrono>
#include <list>
#include <memory>

namespace ECAppLogCPP
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

class Client
{
public:
	Client(const std::string &appName = "ECAPPLOG-CPP", const std::string& address = "127.0.0.1", uint16_t port = 13991);

	void open();
	void close();

	void log(const std::chrono::system_clock::time_point& time, const std::string &priority, const std::string &message,
		const std::string &source = "", const std::list<std::string> &extraCategories = std::list<std::string>());
	void logNow(const std::string& priority, const std::string& message,
		const std::string& source = "", const std::list<std::string>& extraCategories = std::list<std::string>());
private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};

}
