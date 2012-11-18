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
	typename Container::iterator i = c.erase(c.begin());

	REQUIRE(c.size() == 7);
	REQUIRE(c.front() == 1);
	REQUIRE(i == c.begin());

	i = c.erase(c.begin(), c.end());

	REQUIRE(c.empty() == true);
	REQUIRE(i == c.begin());
	REQUIRE(i == c.end());
}

API_TEST_CASE("API/push_back", "Pushing values. Push it! Push! Push!", push_back)
{
	Container c;

	// Value push_back.
	for(const auto& a : zero_to_seven)
	{
		c.push_back(a);
	}

	REQUIRE(equal(c.begin(), c.end(), zero_to_seven.begin()));

	// Range push_back.
	c.clear();
	c.push_back(zero_to_seven.begin(), zero_to_seven.end());

	REQUIRE(equal(c.begin(), c.end(), zero_to_seven.begin()));
}

API_TEST_CASE("API/find", "Elements from container can be found. Elements not from container cannot be found.", find)
{
	Container c(begin(zero_to_seven), end(zero_to_seven));
	typename Container::iterator i;

	for(const auto& a : zero_to_seven)
	{
		i = c.find(a);
		REQUIRE(i != c.end());
	}

	i = c.find(8);
	REQUIRE(i == c.end());
}

API_TEST_CASE("API/find_if", "Elements from container can be found with a predicate. Elements not from container cannot be found.", find_if)
{
	Container c(begin(zero_to_seven), end(zero_to_seven));
	typename Container::iterator i;

	for(const auto& a : zero_to_seven)
	{
		i = c.find_if([a](const typename Container::value_type& v){ return v == a; });
		REQUIRE(i != c.end());
	}

	i = c.find_if([](const typename Container::value_type& v){ return v == 8; });
	REQUIRE(i == c.end());
}

#if defined(__GNUC__) || defined(__clang__)

template<typename Policy>
bool policy_check(vector<int> starting_order, vector<int> find_order, vector<int> final_order)
{
	{
		so::vector<int, Policy> v(begin(starting_order), end(starting_order));

		for(const auto& i : find_order)
		{
			v.find(i);
		}

		CHECK(equal(v.begin(), v.end(), begin(final_order)));
	}

	{
		so::list<int, Policy> l(begin(starting_order), end(starting_order));

		for(const auto& i : find_order)
		{
			l.find(i);
		}

		CHECK(equal(begin(l), end(l), begin(final_order)));
	}
}

TEST_CASE("policy/empty", "An empty container remains empty after searching anything.")
{
	policy_check<so::find_policy::count>({}, {1, 2, 3}, {});

	policy_check<so::find_policy::transpose>({}, {1, 2, 3}, {});

	policy_check<so::find_policy::move_to_front>({}, {1, 2, 3}, {});
}

TEST_CASE("policy/no_hits", "A container remains the same after searching for non-existing values.")
{
	policy_check<so::find_policy::count>({1, 2, 3}, {0, 0, 0}, {1, 2, 3});

	policy_check<so::find_policy::transpose>({1, 2, 3}, {0, 0, 0}, {1, 2, 3});

	policy_check<so::find_policy::move_to_front>({1, 2, 3}, {0, 0, 0}, {1, 2, 3});
}

TEST_CASE("policy/count_one", "An elements searched for once ends up at the front of the container.")
{
	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {0}, {0, 1, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {1}, {1, 0, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {2}, {2, 0, 1, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {6}, {6, 0, 1, 2, 3, 4, 5, 7});

	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {7}, {7, 0, 1, 2, 3, 4, 5, 6});
}

TEST_CASE("policy/count_three", "An element searched for three times ends up at the front of the container.")
{
	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {0, 0, 0}, {0, 1, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {1, 1, 1}, {1, 0, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {2, 2, 2}, {2, 0, 1, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {6, 6, 6}, {6, 0, 1, 2, 3, 4, 5, 7});

	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {7, 7, 7}, {7, 0, 1, 2, 3, 4, 5, 6});
}

TEST_CASE("policy/count_order", "Elements searched in order remain at their original position.")
{
	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {0, 1, 2, 3, 4, 5, 6, 7}, {0, 1, 2, 3, 4, 5, 6, 7});			
}

TEST_CASE("policy/reverse_order", "Elements searched in reverse order remain at their new position.")
{
	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {7, 6, 5, 4, 3, 2, 1, 0}, {7, 6, 5, 4, 3, 2, 1, 0});			
}

TEST_CASE("policy/count_random", "Elements searched once or multiple times produced the determined final order.")
{
	policy_check<so::find_policy::count>({0, 1, 2, 3, 4, 5, 6, 7}, {5, 3, 5, 6, 4, 6, 5, 0, 3, 5, 6, 4}, {5, 6, 3, 4, 0, 1, 2, 7});			
}

TEST_CASE("policy/move_to_front_one", "An elements searched for once ends up at the front of the container.")
{
	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {0}, {0, 1, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {1}, {1, 0, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {2}, {2, 0, 1, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {6}, {6, 0, 1, 2, 3, 4, 5, 7});

	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {7}, {7, 0, 1, 2, 3, 4, 5, 6});
}

TEST_CASE("policy/move_to_front_three", "An elements searched for three times ends up at the front of the container.")
{
	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {0, 0, 0}, {0, 1, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {1, 1, 1}, {1, 0, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {2, 2, 2}, {2, 0, 1, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {6, 6, 6}, {6, 0, 1, 2, 3, 4, 5, 7});

	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {7, 7, 7}, {7, 0, 1, 2, 3, 4, 5, 6});
}

TEST_CASE("policy/move_to_front_order", "Elements searched in order remain at their new position.")
{
	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {0, 1, 2, 3, 4, 5, 6, 7}, {7, 6, 5, 4, 3, 2, 1, 0});			
}

TEST_CASE("policy/move_to_front_reverse_order", "Elements searched in reverse order remain at their original position.")
{
	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {7, 6, 5, 4, 3, 2, 1, 0}, {0, 1, 2, 3, 4, 5, 6, 7});			
}

TEST_CASE("policy/move_to_front_random", "Elements searched once or multiple times produced the determined final order.")
{
	policy_check<so::find_policy::move_to_front>({0, 1, 2, 3, 4, 5, 6, 7}, {5, 3, 5, 6, 4, 6, 5, 0, 3, 5, 6, 4}, {4, 6, 5, 3, 0, 1, 2, 7});			
}

TEST_CASE("policy/transpose_one", "An elements searched for once is swapped with the element preceding it.")
{
	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {0}, {0, 1, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {1}, {1, 0, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {2}, {0, 2, 1, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {6}, {0, 1, 2, 3, 4, 6, 5, 7});

	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {7}, {0, 1, 2, 3, 4, 5, 7, 6});
}

TEST_CASE("policy/transpose_three", "An elements searched for three times moves forward three positions.")
{
	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {0, 0, 0}, {0, 1, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {1, 1, 1}, {1, 0, 2, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {2, 2, 2}, {2, 0, 1, 3, 4, 5, 6, 7});

	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {6, 6, 6}, {0, 1, 2, 6, 3, 4, 5, 7});

	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {7, 7, 7}, {0, 1, 2, 3, 7, 4, 5, 6});
}

TEST_CASE("policy/transpose_order", "Elements searched in order rotate the container forward.")
{
	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {0, 1, 2, 3, 4, 5, 6, 7}, {1, 2, 3, 4, 5, 6, 7, 0});
}

TEST_CASE("policy/transpose_reverse_order", "Elements searched in reverse order remain at their original position.")
{
	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {7, 6, 5, 4, 3, 2, 1, 0}, {0, 1, 2, 3, 4, 5, 6, 7});			
}
TEST_CASE("policy/transpose_random", "Elements searched once or multiple times produced the determined final order.")
{
	policy_check<so::find_policy::transpose>({0, 1, 2, 3, 4, 5, 6, 7}, {5, 3, 5, 6, 4, 6, 5, 0, 3, 5, 6, 4}, {0, 1, 5, 3, 6, 4, 2, 7});			
}

#endif
