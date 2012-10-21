file(GLOB headers *.h)
file(GLOB sources *.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include) 

add_executable(functional ${headers} ${sources})

set_property(TARGET functional PROPERTY FOLDER "tests")

add_test(final_order_list_count functional final_order_list_count)
add_test(final_order_list_move_to_front functional final_order_list_move_to_front)
add_test(final_order_list_transpose functional final_order_list_transpose)

add_test(final_order_vector_count functional final_order_vector_count)
add_test(final_order_vector_move_to_front functional final_order_vector_move_to_front)
add_test(final_order_vector_transpose functional final_order_vector_transpose)