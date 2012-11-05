include_directories(${CMAKE_INCLUDE_PATH} ${PROJECT_SOURCE_DIR}/include) 

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS -std=c++0x)
endif()

add_executable(examples example.cpp)
