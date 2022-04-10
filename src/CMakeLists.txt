set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

add_library(ecapplogcpp-objs OBJECT
	${INCLUDE_DIR}/ecapplogcpp/Client.h
	Client.cpp
	Cmd.h
	Cmd.cpp
	SafeQueue.h
)

target_compile_features(ecapplogcpp-objs PUBLIC cxx_std_17)

# This is only necessary for older compilers, but doesn't hurt
set_target_properties(ecapplogcpp-objs PROPERTIES POSITION_INDEPENDENT_CODE 1)
