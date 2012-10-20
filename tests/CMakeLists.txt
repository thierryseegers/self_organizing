file(GLOB headers *.h)
file(GLOB sources *.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include) 

add_executable(functional ${headers} ${sources})

set_property(TARGET functional PROPERTY FOLDER "tests")

add_test(final_order_zero_to_seven_count functional final_order_zero_to_seven_count)
add_test(final_order_zero_to_seven_move_to_front functional final_order_zero_to_seven_move_to_front)
add_test(final_order_zero_to_seven_transpose functional final_order_zero_to_seven_transpose)