cmake_minimum_required(VERSION 2.8)

project(self_organizing)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_GENERATOR STREQUAL Xcode)
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif()

enable_testing()

add_subdirectory(documentation)
add_subdirectory(examples)
add_subdirectory(include)
add_subdirectory(tests)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION . FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ${PROJECT_BINARY_DIR}/documentation/htdocs DESTINATION documentation)

set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_FILE_NAME self_organizing_${VERSION_MAJOR}_${VERSION_MINOR}) 
include(CPack)