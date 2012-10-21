#include "self_organizing.h"

#include <array>
#include <cstring>
#include <iostream>
#include <map>
#include <string>

using namespace std;

namespace so = self_organizing;

const array<int, 8> zero_to_seven = {0, 1, 2, 3, 4, 5, 6, 7};

typedef map<string, string> args_t;

args_t make_args(const char* types[], char* values[], int c)
{
	args_t args;

	while(c)
	{
		--c;

		args[types[c]] = values[c];
	}

	return args;
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

int main(int argc, char* argv[])
{
	bool b = false;

	if(strcmp(argv[1], "final_order_list_count") == 0)
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
