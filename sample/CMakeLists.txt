set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ecapplogcpp_sample main.cpp)
target_link_libraries(ecapplogcpp_sample ecapplogcpp)
