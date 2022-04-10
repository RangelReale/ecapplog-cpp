cmake_minimum_required(VERSION 3.12.0)

project(ecapplogcpp LANGUAGES CXX VERSION "1.0.0")

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET (ARCHIVE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
#LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

## library name
set(ECAPPLOGCPP ecapplogcpp)
set(ECAPPLOGCPP_STATIC ${ECAPPLOGCPP}-static)

# --- Build Options ---

option(ECAPPLOGCPP_BUILD_STATIC "Build static library" ON)
option(ECAPPLOGCPP_BUILD_SHARED "Build shared library" OFF)
option(ECAPPLOGCPP_BUILD_SAMPLES "Build samples" OFF)

# --- Common Library Sources, etc ---

add_subdirectory(src)

# --- External content ---

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

set(SOCKPP_BUILD_STATIC ON CACHE INTERNAL "Build static")
set(SOCKPP_BUILD_SHARED OFF CACHE INTERNAL "Don't build shared")
FetchContent_MakeAvailable(sockpp)
FetchContent_MakeAvailable(json)

# --- Header Locations ---
target_include_directories(ecapplogcpp-objs 
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	PRIVATE
		$<TARGET_PROPERTY:sockpp-static,INCLUDE_DIRECTORIES>
		$<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>
		src
		src/external
)

# --- System libraries ---

include(GNUInstallDirs)

if(WIN32)
	set(LIBS_SYSTEM ws2_32)
elseif(UNIX)
	set(LIBS_SYSTEM c stdc++)
endif()

## --- create the shared library ---

if(ECAPPLOGCPP_BUILD_SHARED)
	message(STATUS "Creating shared library: ${ECAPPLOGCPP}")

	add_library(${ECAPPLOGCPP} SHARED $<TARGET_OBJECTS:ecapplogcpp-objs>)

	## add dependencies to the shared library
	target_link_libraries(${ECAPPLOGCPP}
		PUBLIC
			${LIBS_SYSTEM}
		PRIVATE
			sockpp-static 
			nlohmann_json::nlohmann_json
	)

	target_include_directories(${ECAPPLOGCPP} 
		PUBLIC 
			$<TARGET_PROPERTY:ecapplogcpp-objs,INCLUDE_DIRECTORIES>
	)

	target_compile_features(${ECAPPLOGCPP} PUBLIC
		cxx_std_17
	 )

	set_target_properties(${ECAPPLOGCPP} PROPERTIES
		VERSION ${PROJECT_VERSION}
		SOVERSION ${PROJECT_VERSION_MAJOR})

	install(TARGETS ${ECAPPLOGCPP}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

endif()

# --- Create the static library ---

if(ECAPPLOGCPP_BUILD_STATIC)
	message(STATUS "Creating static library: ${ECAPPLOGCPP_STATIC}")

	add_library(${ECAPPLOGCPP_STATIC} STATIC $<TARGET_OBJECTS:ecapplogcpp-objs>)

	## add dependencies to the static library
	target_link_libraries(${ECAPPLOGCPP_STATIC} 
		PUBLIC
			${LIBS_SYSTEM}
		PRIVATE
			sockpp-static 
			nlohmann_json::nlohmann_json
	)

	target_include_directories(${ECAPPLOGCPP_STATIC} 
		PUBLIC 
			$<TARGET_PROPERTY:ecapplogcpp-objs,INCLUDE_DIRECTORIES>
	)

	target_compile_features(${ECAPPLOGCPP_STATIC} PUBLIC
		cxx_std_17
	 )

	install(TARGETS ${ECAPPLOGCPP_STATIC}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

	# Let the archive use the same name as the shared library
	if(UNIX)
		set_target_properties(${ECAPPLOGCPP_STATIC} PROPERTIES OUTPUT_NAME ${ECAPPLOGCPP})
	endif()
endif()

# --- Install the library ---

install(DIRECTORY include/ecapplogcpp
	DESTINATION include/ecapplogcpp
	FILES_MATCHING PATTERN "*.h*")

# --- Default library for examples and unit tests ---

if(ECAPPLOGCPP_BUILD_SHARED)
	set(ECAPPLOGCPP_LIB ${ECAPPLOGCPP})
else()
	set(ECAPPLOGCPP_LIB ${ECAPPLOGCPP_STATIC})
endif()

# --- Examples Applications ---

if (ECAPPLOGCPP_BUILD_SAMPLES)
  add_subdirectory(sample)
endif()
