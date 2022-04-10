cmake_minimum_required(VERSION 3.11.0)

project(ECAppLogCPP LANGUAGES CXX VERSION 1.0.0)

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

option(BUILD_SAMPLE "Build sample" OFF)

include_directories(include)

add_subdirectory(src)
if (BUILD_SAMPLE)
  add_subdirectory(sample)
endif()
