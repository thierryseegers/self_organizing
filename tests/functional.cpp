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

template<typename FindPolicy, typename InsertionPolicy>
bool final_order_zero_to_seven(const array<int, 8> final_order)
{
	so::list<int, FindPolicy, InsertionPolicy> l;

	l.insert(begin(zero_to_seven), end(zero_to_seven));

	l.find(5);
	l.find(3);
	l.find(5);
	l.find(6);
	l.find(4);
	l.find(6);
	l.find(5);
	l.find(0);
	l.find(3);
	l.find(5);
	l.find(6);
	l.find(4);

	return equal(begin(l), end(l), begin(final_order));
}

bool final_order_zero_to_seven_count()
{
	const array<int, 8> final_order = {5, 6, 3, 4, 0, 1, 2, 7};
	return final_order_zero_to_seven<so::find_policy::count, so::insertion_policy::count>(final_order);
}

bool final_order_zero_to_seven_move_to_front()
{
	const array<int, 8> final_order = {4, 6, 5, 3, 0, 1, 2, 7};
	return final_order_zero_to_seven<so::find_policy::move_to_front, so::insertion_policy::insert_back>(final_order);
}

bool final_order_zero_to_seven_transpose()
{
	const array<int, 8> final_order = {0, 1, 5, 3, 6, 4, 2, 7};
	return final_order_zero_to_seven<so::find_policy::transpose, so::insertion_policy::insert_back>(final_order);
}

int main(int argc, char* argv[])
{
	bool b = false;

	if(strcmp(argv[1], "final_order_zero_to_seven_count") == 0)
	{
		b = final_order_zero_to_seven_count();
	}
	else if(strcmp(argv[1], "final_order_zero_to_seven_move_to_front") == 0)
	{
		b = final_order_zero_to_seven_move_to_front();
	}
	else if(strcmp(argv[1], "final_order_zero_to_seven_transpose") == 0)
	{
		b = final_order_zero_to_seven_transpose();
	}

	return b ? 0 : 1;
}
