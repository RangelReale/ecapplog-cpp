list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
include(CTest)
include(Catch)

add_executable(ecapplogcpp_tests
  test_chrono.cpp
)
add_test(NAME UnitTests COMMAND ecapplogcpp_tests)

target_include_directories(ecapplogcpp_tests
	PRIVATE
		../src/external
)

target_link_libraries(ecapplogcpp_tests 
  PUBLIC 
    ${ECAPPLOGCPP_LIB}
  PRIVATE 
    Catch2::Catch2WithMain    
)
    
catch_discover_tests(ecapplogcpp_tests)
