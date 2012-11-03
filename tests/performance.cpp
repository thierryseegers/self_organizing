#include "self_organizing.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <functional>
#include <future>
#include <iostream>
#include <random>

using namespace std;

namespace so = self_organizing;

template<template<typename, typename> class C, template<typename, typename> class S>
void test(size_t size, const double sigma)
{
	static vector<int> v(size);
	static bool initialized = false;
	
	if(!initialized)
	{
		for(size_t i = 0; i != size; ++i)
		{
			v[i] = i;
		}

		srand(unsigned int(time(0)));
		random_shuffle(begin(v), end(v));

		initialized = true;
	}

	// Initialize a control container with the numbers from 0 to size, shuffled.
	C<int, std::allocator<int>> control(begin(v), end(v));

	// Initialize three containers with different policies with the same values as the control container.
	S<int, so::find_policy::count> count(begin(control), end(control));
	S<int, so::find_policy::transpose> transpose(begin(control), end(control));
	S<int, so::find_policy::move_to_front> mtf(begin(control), end(control));

	// Generate 100000 numbers to search for given the passed distribution.
	random_device rd;
    mt19937 gen(rd());
	normal_distribution<> dis(size / 2, sigma);

	vector<int> searches;
	generate_n(back_inserter(searches), 100000, [&dis, &gen]{ return dis(gen); });


	// Perform the for the numbers on all four containers, in parallel.
	chrono::high_resolution_clock hrc;

	auto control_time = async(launch::async, [&hrc, &searches, &control]()
	{
		auto start = hrc.now();

		for(size_t i = 0; i != searches.size(); ++i)
		{
			find(begin(control), end(control), searches[i]);
		}
	
		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto count_time = async(launch::async, [&hrc, &searches, &count]()
	{
		auto start = hrc.now();

		for(size_t i = 0; i != searches.size(); ++i)
		{
			count.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto transpose_time = async(launch::async, [&hrc, &searches, &transpose]()
	{
		auto start = hrc.now();

		for(size_t i = 0; i != searches.size(); ++i)
		{
			transpose.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto start = hrc.now();

	for(size_t i = 0; i != searches.size(); ++i)
	{
		mtf.find(searches[i]);
	}
	
	cout << "find_policy::move_to_front: " << chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count() << "ms" << endl;
	cout << "find_policy::transpose: " << transpose_time.get() << "ms" << endl;
	cout << "find_policy::count: " << count_time.get() << "ms" << endl;
	cout << "std::control: " << control_time.get() << "ms" << endl;
}

int main()
{
	cout << "list<>" << endl;

	for(double sigma = 15000.; sigma > 4999.; sigma -= 5000)
	{
		cout << "Sigma: " << sigma << endl;
		test<std::list, so::list>(100000, sigma);
		cout << endl;
	}

	for(double sigma = 4000.; sigma > 999.; sigma -= 1000)
	{
		cout << "Sigma: " << sigma << endl;
		test<std::list, so::list>(100000, sigma);
		cout << endl;
	}

	cout << endl;
	cout << "vector<>" << endl;

	for(double sigma = 15000.; sigma > 4999.; sigma -= 5000)
	{
		cout << "Sigma: " << sigma << endl;
		test<std::vector, so::vector>(100000, sigma);
		cout << endl;
	}

	for(double sigma = 4000.; sigma > 999.; sigma -= 1000)
	{
		cout << "Sigma: " << sigma << endl;
		test<std::vector, so::vector>(100000, sigma);
		cout << endl;
	}

	return 0;
}
