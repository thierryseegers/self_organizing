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
#include <set>

using namespace std;

namespace so = self_organizing;

void test(size_t size, const double sigma, const bool sorted = false)
{
	static vector<int> v(size);
	static bool initialized = false;
	
	if(!initialized)
	{
		for(size_t i = 0; i != size; ++i)
		{
			v[i] = i;
		}

		srand((unsigned int)time(0));
		random_shuffle(begin(v), end(v));

		initialized = true;
	}

	// Initialize control containers with the numbers from 0 to size, shuffled.
	list<int> std_l(begin(v), end(v));
	vector<int> std_v(begin(v), end(v));
	set<int> std_s(begin(v), end(v));

	// Initialize three so::lists and three so::vector with different policies with the same values as the control container.
	so::list<int, so::find_policy::count> so_l_count(begin(v), end(v));
	so::list<int, so::find_policy::transpose> so_l_transpose(begin(v), end(v));
	so::list<int, so::find_policy::move_to_front> so_l_mtf(begin(v), end(v));
	so::vector<int, so::find_policy::count> so_v_count(begin(v), end(v));
	so::vector<int, so::find_policy::transpose> so_v_transpose(begin(v), end(v));
	so::vector<int, so::find_policy::move_to_front> so_v_mtf(begin(v), end(v));

	// Generate 100000 numbers to search for given the passed distribution.
	random_device rd;
	mt19937 gen(rd());
	normal_distribution<> dis(size / 2, sigma);

	vector<int> searches;
	generate_n(back_inserter(searches), 100000, [&dis, &gen]{ return dis(gen); });

	if(sorted) sort(searches.begin(), searches.end());

	chrono::high_resolution_clock hrc;

	// Perform the search on the control group.
	auto std_l_time = async(launch::deferred, [&hrc, &searches, &std_l]()
	{
		auto start = hrc.now();

		for(size_t i = 0; i != searches.size(); ++i)
		{
			find(begin(std_l), end(std_l), searches[i]);
		}
	
		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto std_v_time = async(launch::deferred, [&hrc, &searches, &std_v]()
	{
		auto start = hrc.now();

		for(size_t i = 0; i != searches.size(); ++i)
		{
			find(begin(std_v), end(std_v), searches[i]);
		}
	
		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});
	
	auto start = hrc.now();

	for(size_t i = 0; i != searches.size(); ++i)
	{
		std_s.find(searches[i]);
	}
	
	cout << "std::set: " << chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count() << "ms" << endl;
	cout << "std::vector: " << std_v_time.get() << "ms" << endl;
	cout << "std::list: " << std_l_time.get() << "ms" << endl;


	// Perform the search on the self-organizing containers.
	auto so_l_count_time = async(launch::deferred, [&hrc, &searches, &so_l_count]()
	{
		auto start = hrc.now();

		for(size_t i = 0; i != searches.size(); ++i)
		{
			so_l_count.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto so_l_transpose_time = async(launch::deferred, [&hrc, &searches, &so_l_transpose]()
	{
		auto start = hrc.now();

		for(size_t i = 0; i != searches.size(); ++i)
		{
			so_l_transpose.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto so_l_mtf_time = async(launch::deferred, [&hrc, &searches, &so_l_mtf]()
	{
		auto start = hrc.now();

		for(size_t i = 0; i != searches.size(); ++i)
		{
			so_l_mtf.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto so_v_count_time = async(launch::deferred, [&hrc, &searches, &so_v_count]()
	{
		auto start = hrc.now();

		for(size_t i = 0; i != searches.size(); ++i)
		{
			so_v_count.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto so_v_transpose_time = async(launch::deferred, [&hrc, &searches, &so_v_transpose]()
	{
		auto start = hrc.now();

		for(size_t i = 0; i != searches.size(); ++i)
		{
			so_v_transpose.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	start = hrc.now();

	for(size_t i = 0; i != searches.size(); ++i)
	{
		so_v_mtf.find(searches[i]);
	}
	
	cout << "so::v<move_to_front>: " << chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count() << "ms" << endl;
	cout << "so::v<transpose>: " << so_v_transpose_time.get() << "ms" << endl;
	cout << "so::v<count>: " << so_v_count_time.get() << "ms" << endl;
	cout << "so::l<move_to_front>: " << so_l_mtf_time.get() << "ms" << endl;
	cout << "so::l<transpose>: " << so_l_transpose_time.get() << "ms" << endl;
	cout << "so::l<count>: " << so_l_count_time.get() << "ms" << endl;
}

int main()
{
	for(double sigma = 15000.; sigma > 4999.; sigma -= 5000)
	{
		cout << "Sigma: " << sigma << endl;
		test(100000, sigma, true);
		cout << endl;
	}

	for(double sigma = 4000.; sigma > 999.; sigma -= 1000)
	{
		cout << "Sigma: " << sigma << endl;
		test(100000, sigma, true);
		cout << endl;
	}

	return 0;
}
