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

template<typename Container>
void empty()
{
	Container c;

	REQUIRE(c.size() == 0);
	REQUIRE(c.begin() == c.end());
}

TEST_CASE("correctness/empty", "Empty container has size 0 and begin() equals end().")
{
	SECTION("list<int, count, count>", "list<int, count, count>")
	{
		empty<so::list<int, so::find_policy::count, so::insertion_policy::count>>();
	}

	SECTION("list<int, move_to_front, insert_front>", "list<int, move_to_front, insert_front>")
	{
		empty<so::list<int, so::find_policy::move_to_front, so::insertion_policy::insert_front>>();
	}
	
	SECTION("list<int, transpose, insert_back>", "list<int, transpose, insert_back>")
	{
		empty<so::list<int, so::find_policy::transpose, so::insertion_policy::insert_back>>();
	}

	SECTION("vector<int, count, count>", "vector<int, count, count>")
	{
		empty<so::vector<int, so::find_policy::count, so::insertion_policy::count>>();
	}

	SECTION("vector<int, move_to_front, insert_front>", "vector<int, move_to_front, insert_front>")
	{
		empty<so::vector<int, so::find_policy::move_to_front, so::insertion_policy::insert_front>>();
	}
	
	SECTION("vector<int, transpose, insert_back>", "vector<int, transpose, insert_back>")
	{
		empty<so::vector<int, so::find_policy::transpose, so::insertion_policy::insert_back>>();
	}
}

template<typename Container>
void range_constructor()
{
	Container c(begin(zero_to_seven), end(zero_to_seven));

	CHECK(equal(begin(c), end(c), begin(zero_to_seven)));
}

TEST_CASE("correctness/range_constructor", "Constructing container with range equals range.")
{
	SECTION("list<int, count, count>", "list<int, count, count>")
	{
		range_constructor<so::list<int, so::find_policy::count, so::insertion_policy::count>>();
	}

	SECTION("list<int, move_to_front, insert_front>", "list<int, move_to_front, insert_front>")
	{
		range_constructor<so::list<int, so::find_policy::move_to_front, so::insertion_policy::insert_front>>();
	}
	
	SECTION("list<int, transpose, insert_back>", "list<int, transpose, insert_back>")
	{
		range_constructor<so::list<int, so::find_policy::transpose, so::insertion_policy::insert_back>>();
	}

	SECTION("vector<int, count, count>", "vector<int, count, count>")
	{
		range_constructor<so::vector<int, so::find_policy::count, so::insertion_policy::count>>();
	}

	SECTION("vector<int, move_to_front, insert_front>", "vector<int, move_to_front, insert_front>")
	{
		range_constructor<so::vector<int, so::find_policy::move_to_front, so::insertion_policy::insert_front>>();
	}
	
	SECTION("vector<int, transpose, insert_back>", "vector<int, transpose, insert_back>")
	{
		range_constructor<so::vector<int, so::find_policy::transpose, so::insertion_policy::insert_back>>();
	}
}

template<typename Container>
void copy_constructor()
{
	Container c(begin(zero_to_seven), end(zero_to_seven)), copy(c);

	CHECK(equal(begin(c), end(c), begin(copy)));
}

TEST_CASE("correctness/copy_constructor", "Copy-constructing container equals copy.")
{
	SECTION("list<int, count, count>", "list<int, count, count>")
	{
		copy_constructor<so::list<int, so::find_policy::count, so::insertion_policy::count>>();
	}

	SECTION("list<int, move_to_front, insert_front>", "list<int, move_to_front, insert_front>")
	{
		copy_constructor<so::list<int, so::find_policy::move_to_front, so::insertion_policy::insert_front>>();
	}
	
	SECTION("list<int, transpose, insert_back>", "list<int, transpose, insert_back>")
	{
		copy_constructor<so::list<int, so::find_policy::transpose, so::insertion_policy::insert_back>>();
	}

	SECTION("vector<int, count, count>", "vector<int, count, count>")
	{
		copy_constructor<so::vector<int, so::find_policy::count, so::insertion_policy::count>>();
	}

	SECTION("vector<int, move_to_front, insert_front>", "vector<int, move_to_front, insert_front>")
	{
		copy_constructor<so::vector<int, so::find_policy::move_to_front, so::insertion_policy::insert_front>>();
	}
	
	SECTION("vector<int, transpose, insert_back>", "vector<int, transpose, insert_back>")
	{
		copy_constructor<so::vector<int, so::find_policy::transpose, so::insertion_policy::insert_back>>();
	}
}
/*
TEST_CASE("correctness/insert", "Container equals insert values.")
{
	SECTION("list<int, count, count>", "list<int, count, count>")
	{
		so::list<int, so::find_policy::count, so::insertion_policy::count> l(begin(zero_to_seven), end(zero_to_seven));

	}

	SECTION("list<int, move_to_front, insert_front>", "list<int, move_to_front, insert_front>")
	{
		insert<so::list<int, so::find_policy::move_to_front, so::insertion_policy::insert_front>>();
	}
	
	SECTION("list<int, transpose, insert_back>", "list<int, transpose, insert_back>")
	{
		insert<so::list<int, so::find_policy::transpose, so::insertion_policy::insert_back>>();
	}

	SECTION("vector<int, count, count>", "vector<int, count, count>")
	{
		insert<so::vector<int, so::find_policy::count, so::insertion_policy::count>>();
	}

	SECTION("vector<int, move_to_front, insert_front>", "vector<int, move_to_front, insert_front>")
	{
		insert<so::vector<int, so::find_policy::move_to_front, so::insertion_policy::insert_front>>();
	}
	
	SECTION("vector<int, transpose, insert_back>", "vector<int, transpose, insert_back>")
	{
		insert<so::vector<int, so::find_policy::transpose, so::insertion_policy::insert_back>>();
	}
}

/*

template<typename Container>
bool size()
{
	Container c;

	c.insert(begin(zero_to_seven), end(zero_to_seven));

	return c.size() == 8;
}

bool size_list()
{
	return size<so::list<int, so::find_policy::count, so::insertion_policy::count>>();
}

bool size_vector()
{
	return size<so::vector<int, so::find_policy::count, so::insertion_policy::count>>();
}

template<typename Container>
bool front_back()
{
	Container c;

	c.insert(begin(zero_to_seven), end(zero_to_seven));

	if(c.front() != 0)
	{
		return false;
	}

	if(c.back() != 7)
	{
		return false;
	}

	return true;
}

bool front_back_list()
{
	return front_back<so::list<int, so::find_policy::count, so::insertion_policy::count>>();
}

bool front_back_vector()
{
	return front_back<so::vector<int, so::find_policy::count, so::insertion_policy::count>>();
}

template<typename Container>
bool clear()
{
	Container c;

	c.insert(begin(zero_to_seven), end(zero_to_seven));

	c.clear();

	if(c.size() != 0)
	{
		return false;
	}

	if(c.begin() != c.end())
	{
		return false;
	}

	return true;
}

bool clear_list()
{
	return clear<so::list<int, so::find_policy::count, so::insertion_policy::count>>();
}

bool clear_vector()
{
	return clear<so::vector<int, so::find_policy::count, so::insertion_policy::count>>();
}

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
	return final_order<so::list<int, so::find_policy::count, so::insertion_policy::count>>(order);
}

bool final_order_list_move_to_front()
{
	const array<int, 8> order = {4, 6, 5, 3, 0, 1, 2, 7};
	return final_order<so::list<int, so::find_policy::move_to_front, so::insertion_policy::insert_back>>(order);
}

bool final_order_list_transpose()
{
	const array<int, 8> order = {0, 1, 5, 3, 6, 4, 2, 7};
	return final_order<so::list<int, so::find_policy::transpose, so::insertion_policy::insert_back>>(order);
}

bool final_order_vector_count()
{
	const array<int, 8> order = {5, 6, 3, 4, 0, 1, 2, 7};
	return final_order<so::vector<int, so::find_policy::count, so::insertion_policy::count>>(order);
}

bool final_order_vector_move_to_front()
{
	const array<int, 8> order = {4, 6, 5, 3, 0, 1, 2, 7};
	return final_order<so::vector<int, so::find_policy::move_to_front, so::insertion_policy::insert_back>>(order);
}

bool final_order_vector_transpose()
{
	const array<int, 8> order = {0, 1, 5, 3, 6, 4, 2, 7};
	return final_order<so::vector<int, so::find_policy::transpose, so::insertion_policy::insert_back>>(order);
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