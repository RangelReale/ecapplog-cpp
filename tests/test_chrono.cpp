#include "ecapplogcpp/Client.h"
#include "catch2/catch.hpp"
#include "date.h"

#include <string>
#include <chrono>
#include <iostream>

using namespace ecapplogcpp;

TEST_CASE("mktime", "[Client]") {
	auto tp = Client::mktime(2020, 2, 15, 10, 17, 20, 120);

    auto dp = date::floor<date::days>(tp);
    date::year_month_day ymd{ dp };
    date::hh_mm_ss time{date::floor<std::chrono::milliseconds>(tp - dp)};

	REQUIRE( ymd.year() == date::year(2020) );
    REQUIRE( ymd.month() == date::month(2) );
    REQUIRE( ymd.day() == date::day(15) );
    REQUIRE( time.hours() == std::chrono::hours(10) );
    REQUIRE( time.minutes() == std::chrono::minutes(17) );
    REQUIRE( time.seconds() == std::chrono::seconds(20) );
    REQUIRE( time.subseconds() == std::chrono::milliseconds(120) );
}
