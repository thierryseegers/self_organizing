#include "self_organizing.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <array>
#include <cstring>
#include <iostream>
#include <map>
#include <string>

using namespace std;

namespace so = self_organizing;

const array<int, 8> zero_to_seven = {0, 1, 2, 3, 4, 5, 6, 7};

#define API_TEST_CASE(name, description, function)						\
																		\
template<typename Container>											\
void function();														\
																		\
TEST_CASE(name, description)											\
{																		\
	SECTION("list<int, count, count>", "list<int, count, count>")		\
	{																	\
		function<so::list<int, so::find_policy::count>>();				\
	}																	\
																		\
	SECTION("list<int, move_to_front>", "list<int, move_to_front>")		\
	{																	\
		function<so::list<int, so::find_policy::move_to_front>>();		\
	}																	\
																		\
	SECTION("list<int, transpose>", "list<int, transpose>")				\
	{																	\
		function<so::list<int, so::find_policy::transpose>>();			\
	}																	\
																		\
	SECTION("vector<int, count, count>", "vector<int, count, count>")	\
	{																	\
		function<so::vector<int, so::find_policy::count>>();			\
	}																	\
																		\
	SECTION("vector<int, move_to_front>", "vector<int, move_to_front>")	\
	{																	\
		function<so::vector<int, so::find_policy::move_to_front>>();	\
	}																	\
																		\
	SECTION("vector<int, transpose>", "vector<int, transpose>")			\
	{																	\
		function<so::vector<int, so::find_policy::transpose>>();		\
	}																	\
}																		\
																		\
template<typename Container>											\
void function()


API_TEST_CASE("API/default_constructor", "Dummy test. Confirms compilation is successful.", default_constructor)
{
	Container c;
}

API_TEST_CASE("API/range_constructor", "Range-constructing container equals range.", range_constructor)
{
	Container c(begin(zero_to_seven), end(zero_to_seven));

	CHECK(equal(begin(c), end(c), begin(zero_to_seven)));
}

API_TEST_CASE("API/copy_constructor", "Copy-constructing container equals copy.", copy_constructor)
{
	Container c(begin(zero_to_seven), end(zero_to_seven)), copy(c);

	CHECK(equal(begin(c), end(c), begin(copy)));
}

API_TEST_CASE("API/front_and_back", "Front and back values of constructed container.", front_back)
{
	Container c(begin(zero_to_seven), end(zero_to_seven));

	REQUIRE(c.front() == 0);
	REQUIRE(c.back() == 7);
}

API_TEST_CASE("API/empty", "Empty is true for default container.", empty)
{
	Container c;

	REQUIRE(c.empty() == true);
}

API_TEST_CASE("API/size", "Size of constructed container is correct.", size)
{
	{
		Container c;

		REQUIRE(c.size() == 0);
	}

	{
		Container c(begin(zero_to_seven), end(zero_to_seven));

		REQUIRE(c.size() == 8);
	}
}

API_TEST_CASE("API/clear", "Size of cleared container is 0.", clear)
{
	Container c(begin(zero_to_seven), end(zero_to_seven));
	c.clear();

	REQUIRE(c.size() == 0);
}

API_TEST_CASE("API/erase", "Container shrinks as elements are erased.", erase)
{
	Container c(begin(zero_to_seven), end(zero_to_seven));
	Container::iterator i = c.erase(c.begin());

	REQUIRE(c.size() == 7);
	REQUIRE(c.front() == 1);
	REQUIRE(i == c.begin());

	i = c.erase(c.begin(), c.end());

	REQUIRE(c.empty() == true);
	REQUIRE(i == c.begin());
	REQUIRE(i == c.end());
}

API_TEST_CASE("API/push_back", "Size of cleared container is 0.", push_back)
{
	Container c;
	for(const auto& a : zero_to_seven)
	{
		c.push_back(a);
	}

	REQUIRE(equal(c.begin(), c.end(), zero_to_seven.begin()));
}

API_TEST_CASE("API/find", "Elements from container can be found. Elements not from caontainer cannot be found.", find)
{
	Container c(begin(zero_to_seven), end(zero_to_seven));
	Container::iterator i;

	for(const auto& a : zero_to_seven)
	{
		i = c.find(a);
		REQUIRE(i != c.end());
	}

	i = c.find(8);
	REQUIRE(i == c.end());
}

API_TEST_CASE("API/find_if", "Elements from container can be found with a predicate. Elements not from caontainer cannot be found.", find_if)
{
	Container c(begin(zero_to_seven), end(zero_to_seven));
	Container::iterator i;

	for(const auto& a : zero_to_seven)
	{
		i = c.find_if([a](const Container::value_type& v){ return v == a; });
		REQUIRE(i != c.end());
	}

	i = c.find_if([](const Container::value_type& v){ return v == 8; });
	REQUIRE(i == c.end());
}



/*
template<typename Container>
bool final_order(const array<int, 8> final_order)
{
	Container c;

	c.insert(begin(zero_to_seven), end(zero_to_seven));

	c.find(5);
	c.find(3);
	c.find(5);
	c.find(6);
	c.find(4);
	c.find(6);
	c.find(5);
	c.find(0);
	c.find(3);
	c.find(5);
	c.find(6);
	c.find(4);

	return equal(begin(c), end(c), begin(final_order));
}

bool final_order_list_count()
{
	const array<int, 8> order = {5, 6, 3, 4, 0, 1, 2, 7};
	return final_order<so::list<int, so::find_policy::count>>(order);
}

bool final_order_list_move_to_front()
{
	const array<int, 8> order = {4, 6, 5, 3, 0, 1, 2, 7};
	return final_order<so::list<int, so::find_policy::move_to_front>>(order);
}

bool final_order_list_transpose()
{
	const array<int, 8> order = {0, 1, 5, 3, 6, 4, 2, 7};
	return final_order<so::list<int, so::find_policy::transpose>>(order);
}

bool final_order_vector_count()
{
	const array<int, 8> order = {5, 6, 3, 4, 0, 1, 2, 7};
	return final_order<so::vector<int, so::find_policy::count>>(order);
}

bool final_order_vector_move_to_front()
{
	const array<int, 8> order = {4, 6, 5, 3, 0, 1, 2, 7};
	return final_order<so::vector<int, so::find_policy::move_to_front>>(order);
}

bool final_order_vector_transpose()
{
	const array<int, 8> order = {0, 1, 5, 3, 6, 4, 2, 7};
	return final_order<so::vector<int, so::find_policy::transpose>>(order);
}



TEST_CASE( "stupid/1=2", "Prove that one equals 2" )
{
    int one = 1;
    REQUIRE( one == 2 );
	CHECK( one == 2 );
}

TEST_CASE( "example/less than 7", "The number is less than 7" )
{
    int notThisOne = 7;

    for( int i=0; i < 7; ++i )
    {
        REQUIRE( notThisOne > i+1  );
    }
}
*/
/*
int main(int argc, char* argv[])
{
	bool b = false;

	if(strcmp(argv[1], "empty_list") == 0)
	{
		b = empty_list();
	}
	else if(strcmp(argv[1], "empty_vector") == 0)
	{
		b = empty_vector();
	}
	else if(strcmp(argv[1], "insert_list") == 0)
	{
		b = insert_list();
	}
	else if(strcmp(argv[1], "insert_vector") == 0)
	{
		b = insert_vector();
	}
	else if(strcmp(argv[1], "size_list") == 0)
	{
		b = size_list();
	}
	else if(strcmp(argv[1], "size_vector") == 0)
	{
		b = size_vector();
	}
	else if(strcmp(argv[1], "front_back_list") == 0)
	{
		b = front_back_list();
	}
	else if(strcmp(argv[1], "front_back_vector") == 0)
	{
		b = front_back_vector();
	}
	else if(strcmp(argv[1], "clear_list") == 0)
	{
		b = size_list();
	}
	else if(strcmp(argv[1], "clear_vector") == 0)
	{
		b = size_vector();
	}
	else if(strcmp(argv[1], "final_order_list_count") == 0)
	{
		b = final_order_list_count();
	}
	else if(strcmp(argv[1], "final_order_list_move_to_front") == 0)
	{
		b = final_order_list_move_to_front();
	}
	else if(strcmp(argv[1], "final_order_list_transpose") == 0)
	{
		b = final_order_list_transpose();
	}
	else if(strcmp(argv[1], "final_order_vector_count") == 0)
	{
		b = final_order_vector_count();
	}
	else if(strcmp(argv[1], "final_order_vector_move_to_front") == 0)
	{
		b = final_order_vector_move_to_front();
	}
	else if(strcmp(argv[1], "final_order_vector_transpose") == 0)
	{
		b = final_order_vector_transpose();
	}

	return b ? 0 : 1;
}
*/