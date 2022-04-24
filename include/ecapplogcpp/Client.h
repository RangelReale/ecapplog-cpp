#pragma once

#include <ecapplogcpp/Data.h>

#include <string>
#include <cstdint>
#include <chrono>
#include <list>
#include <memory>

namespace ecapplogcpp
{

class Client
{
public:
	Client(const std::string &appName = "ECAPPLOG-CPP", const std::string& address = "127.0.0.1", uint16_t port = 13991);
	~Client();

	void open();
	void close();

	// time format (UTC): YYYY-MM-DDTHH:NN:SS.LLL
	void log(const std::string &timeStr, const std::string &priority, const std::string& category, 
		const std::string &message, const std::string &source = "", const std::string& color = "", const std::string& bgcolor = "", 
		const std::list<std::string>& extraCategories = std::list<std::string>());
	void logNow(const std::string& priority, const std::string& category, const std::string& message,
		const std::string& source = "", const std::string& color = "", const std::string& bgcolor = "", 
		const std::list<std::string>& extraCategories = std::list<std::string>());
	template<class T>
	void logTime(T time, const std::string& priority, const std::string& category,
		const std::string& message, const std::string& source = "",
		const std::string& color = "", const std::string& bgcolor = "", 
		const std::list<std::string>& extraCategories = std::list<std::string>())
	{
		log(TimeProvider<T>(time).getTime(), priority, category, message, source, color, bgcolor, extraCategories);
	}
private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};

}
