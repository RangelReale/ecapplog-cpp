# ECAppLog-go

C++ client for the [ECAppLog](https://github.com/RangelReale/ecapplog) logging GUI.

## Example

```cpp
#include <ecapplogcpp/Client.h>

#include <iostream>
#include <sstream>
#include <thread>

int main()
{
	ecapplogcpp::Client client;
	client.open();

	std::cout << "Sending logs" << std::endl;

	for (int i = 0; i < 100; i++)
	{
		std::stringstream log1;
		log1 << "First log: " << i;
		client.logNow(ecapplogcpp::Priority::Priority_DEBUG, "app", log1.str());

		std::stringstream log2;
		log2 << "Second log: " << i;
		client.logTime(std::chrono::system_clock::now() - std::chrono::hours(1), ecapplogcpp::Priority::Priority_INFORMATION, "app", log2.str());

		std::stringstream log3;
		log3 << "Third log: " << i;
		client.logNow(ecapplogcpp::Priority::Priority_ERROR, "app", log3.str(), "log 3 source", "", "",
			std::list<std::string>{ "app_third" });
	}

	std::cout << "Finished, waiting 1 second" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
```

## Author

Rangel Reale (rangelreale@gmail.com)
