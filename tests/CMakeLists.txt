include_directories(${PROJECT_SOURCE_DIR}/include)

if(MSVC)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()

add_executable(correctness catch.hpp correctness.cpp)
set_property(TARGET correctness PROPERTY FOLDER "tests")

add_executable(performance performance.cpp)
set_property(TARGET performance PROPERTY FOLDER "tests")

add_test(correctness/default_constructor correctness correctness/default_constructor)
add_test(correctness/range_constructor correctness correctness/range_constructor)
add_test(correctness/copy_constructor correctness correctness/copy_constructor)
add_test(correctness/front_and_back correctness correctness/front_and_back)

#add_test(size_list correctness size_list)
#add_test(size_vector correctness size_vector)
#
#add_test(clear_list correctness clear_list)
#add_test(clear_vector correctness clear_vector)
#
#add_test(final_order_list_count correctness final_order_list_count)
#add_test(final_order_list_move_to_front correctness final_order_list_move_to_front)
#add_test(final_order_list_transpose correctness final_order_list_transpose)
#add_test(final_order_vector_count correctness final_order_vector_count)
#add_test(final_order_vector_move_to_front correctness final_order_vector_move_to_front)
#add_test(final_order_vector_transpose correctness final_order_vector_transpose)

