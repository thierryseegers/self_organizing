include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(correctness correctness.cpp)
set_property(TARGET correctness PROPERTY FOLDER "tests")

add_executable(performance performance.cpp)
set_property(TARGET performance PROPERTY FOLDER "tests")

add_test(empty_list correctness empty_list)
add_test(empty_vector correctness empty_vector)

add_test(insert_list correctness insert_list)
add_test(insert_vector correctness insert_vector)

add_test(size_list correctness size_list)
add_test(size_vector correctness size_vector)

add_test(clear_list correctness clear_list)
add_test(clear_vector correctness clear_vector)

add_test(front_back_list correctness front_back_list)
add_test(front_back_vector correctness front_back_vector)

add_test(final_order_list_count correctness final_order_list_count)
add_test(final_order_list_move_to_front correctness final_order_list_move_to_front)
add_test(final_order_list_transpose correctness final_order_list_transpose)
add_test(final_order_vector_count correctness final_order_vector_count)
add_test(final_order_vector_move_to_front correctness final_order_vector_move_to_front)
add_test(final_order_vector_transpose correctness final_order_vector_transpose)

