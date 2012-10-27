include_directories(${PROJECT_SOURCE_DIR}/include)

if(MSVC)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()

add_executable(correctness catch.hpp correctness.cpp)
set_property(TARGET correctness PROPERTY FOLDER "tests")

add_executable(performance performance.cpp)
set_property(TARGET performance PROPERTY FOLDER "tests")

# API tests.
add_test(API/default_constructor correctness API/default_constructor)
add_test(API/range_constructor correctness API/range_constructor)
add_test(API/copy_constructor correctness API/copy_constructor)
add_test(API/front_and_back correctness API/front_and_back)
add_test(API/empty correctness API/empty)
add_test(API/size correctness API/size)
add_test(API/clear correctness API/clear)
add_test(API/erase correctness API/erase)
add_test(API/push_back correctness API/push_back)
add_test(API/find correctness API/find)
add_test(API/find_if correctness API/find_if)

# Policy tests.
add_test(policy/count correctness policy/count)
add_test(policy/transpose correctness policy/transpose)
add_test(policy/move_to_front correctness policy/move_to_front)
