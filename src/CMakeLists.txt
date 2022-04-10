set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX d)

include(FetchContent)
FetchContent_Declare(
	sockpp
	GIT_REPOSITORY https://github.com/fpagliughi/sockpp
	GIT_TAG        v0.7.1
)
FetchContent_Declare(
	json
	GIT_REPOSITORY https://github.com/nlohmann/json
	GIT_TAG		   v3.10.5
)

set(SOCKPP_BUILD_SHARED OFF CACHE BOOL "Don't build shared")
set(SOCKPP_BUILD_STATIC ON CACHE BOOL "Build static")
FetchContent_MakeAvailable(sockpp)
FetchContent_MakeAvailable(json)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})
include_directories(external)

set(SRCS
	${INCLUDE_DIR}/ECAppLogCPP/Client.h
	Client.cpp
	Cmd.h
	Cmd.cpp
	SafeQueue.h
)

if(WIN32)
	set(LIBS_SYSTEM ws2_32)
elseif(UNIX)
	set(LIBS_SYSTEM c stdc++)
endif()

# library
add_library(ecapplogcpp
	${SRCS}
)
target_link_libraries(ecapplogcpp sockpp-static nlohmann_json::nlohmann_json ${LIBS_SYSTEM})
