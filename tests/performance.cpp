#include "self_organizing.h"

#include <algorithm>
#include <array>
#include <cfloat>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <functional>
#include <future>
#include <iostream>
#include <random>
#include <set>
#include <unordered_set>

using namespace std;

namespace so = self_organizing;

template<typename D>
void test(const vector<int>& data, D dis, const bool sorted = false)
{
	const size_t size = data.size();

	random_device rd;
	mt19937 gen(rd());

	// Generate 100000 numbers to search, for the given distribution.
	// If any outliers are generated, discard and re-generate.
	vector<int> searches;
	generate_n(back_inserter(searches), 100000, [&dis, &size, &gen]()->int{ int r; do{ r = dis(gen); }while(r < 0 || r >= size); return r;});

	// Substitute the generated numbers with values from a randomly generated set.
	// This avoids having the numbers generated above being too predictable or concentrated.
	// e.g. For a geometric distribution, even though the distribution is random, 
	// the most common number generated will *always* be 0, the second will *always* be 1, etc.
	// Thus, this extra step adds randomness to the numbers themselves, rather than just their distribution.
	vector<int> substition(searches.size());
	for(size_t i = 0; i != substition.size(); ++i)
	{
		substition[i] = i;
	}

	shuffle(substition.begin(), substition.end(), gen);

	for(size_t i = 0; i != searches.size(); ++i)
	{
		searches[i] = substition[searches[i]];
	}

	// Sort them, if asked to.
	if(sorted) sort(searches.begin(), searches.end());

	chrono::high_resolution_clock hrc;

	// Perform the search on the control group.
	auto std_l_time = async(launch::deferred, [&]()->long long
	{
		auto start = hrc.now();

		list<int> std_l(data.begin(), data.end());

		for(size_t i = 0; i != searches.size(); ++i)
		{
			find(begin(std_l), end(std_l), searches[i]);
		}
	
		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto std_v_time = async(launch::deferred, [&]()->long long
	{
		auto start = hrc.now();

		vector<int> std_v(data.begin(), data.end());

		for(size_t i = 0; i != searches.size(); ++i)
		{
			find(begin(std_v), end(std_v), searches[i]);
		}
	
		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});
	
	auto std_us_time = async(launch::deferred, [&]()->long long
	{
		long long std_us_time = 0;

		for(int n = 0; n != 25; ++n)
		{
			auto start = hrc.now();

			unordered_set<int> std_us(data.begin(), data.end());

			for(size_t i = 0; i != searches.size(); ++i)
			{
				std_us.find(searches[i]);
			}
	
			std_us_time += chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
		}

		return std_us_time / 25;
	});

	long long std_s_time = 0;

	for(int n = 0; n != 25; ++n)
	{
		auto start = hrc.now();

		set<int> std_s(data.begin(), data.end());

		for(size_t i = 0; i != searches.size(); ++i)
		{
			std_s.find(searches[i]);
		}

		std_s_time += chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	}
	
	std_s_time /= 25;

	cout << std_s_time / 1000. << ",";
	cout << std_us_time.get() / 1000. << ",";
	cout << std_l_time.get() / 1000. << ",";
	cout << std_v_time.get() / 1000. << ",";

	// Perform the search on the self-organizing containers.
	auto so_l_transpose_time = async(launch::deferred, [&]()->long long
	{
		auto start = hrc.now();

		so::list<int, so::find_policy::transpose> so_l_transpose(data.begin(), data.end());
	
		for(size_t i = 0; i != searches.size(); ++i)
		{
			so_l_transpose.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto so_l_mtf_time = async(launch::deferred, [&]()->long long
	{
		long long so_l_mtf_time = 0;

//		for(int n = 0; n != 25; ++n)
		{
			auto start = hrc.now();
		
			so::list<int, so::find_policy::move_to_front> so_l_mtf(data.begin(), data.end());
	
			for(size_t i = 0; i != searches.size(); ++i)
			{
				so_l_mtf.find(searches[i]);
			}

			so_l_mtf_time += chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
		}

		return so_l_mtf_time;// / 25;
	});

	auto so_v_count_time = async(launch::deferred, [&]()->long long
	{
		auto start = hrc.now();

		so::vector<int, so::find_policy::count> so_v_count(data.begin(), data.end());

		for(size_t i = 0; i != searches.size(); ++i)
		{
			so_v_count.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto so_v_transpose_time = async(launch::deferred, [&]()->long long
	{
		auto start = hrc.now();

		so::vector<int, so::find_policy::transpose> so_v_transpose(data.begin(), data.end());

		for(size_t i = 0; i != searches.size(); ++i)
		{
			so_v_transpose.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	auto so_v_mtf_time = async(launch::deferred, [&]()->long long
	{
		auto start = hrc.now();

		so::vector<int, so::find_policy::move_to_front> so_v_mtf(data.begin(), data.end());

		for(size_t i = 0; i != searches.size(); ++i)
		{
			so_v_mtf.find(searches[i]);
		}

		return chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	});

	long long so_l_c_time = 0;

//	for(int n = 0; n != 25; ++n)
	{
		auto start = hrc.now();

		so::list<int, so::find_policy::count> so_l_count(data.begin(), data.end());

		for(size_t i = 0; i != searches.size(); ++i)
		{
			so_l_count.find(searches[i]);
		}

		so_l_c_time += chrono::duration_cast<chrono::milliseconds>(hrc.now() - start).count();
	}

	so_l_c_time;// /= 25;

	
	cout << so_l_c_time / 1000. << ",";
	cout << so_l_transpose_time.get() / 1000. << ",";
	cout << so_l_mtf_time.get() / 1000. << ",";
	cout << so_v_count_time.get() / 1000. << ",";
	cout << so_v_transpose_time.get() / 1000. << ",";
	cout << so_v_mtf_time.get() / 1000.;
	cout << endl;
}

int main()
{
	const size_t size = 100000;
	vector<int> data(size);
	
	for(size_t i = 0; i != size; ++i)
	{
		data[i] = i;
	}


	{
		cout << "sigma,";
		cout << "std::set,std::unordered_set,std::list,std::vector,";
		cout << "so::list<count>,so::list<transpose>,so::list<move-to-front>,";
		cout << "so::vector<count>,so::vector<transpose>,so::vector<move-to-front>" << endl;

		for(double sigma = 15000.; sigma > 4999.; sigma -= 5000)
		{
			cout << sigma << ",";
			test(data, normal_distribution<>(data.size() / 2, sigma), true);
		}

		for(double sigma = 4000.; sigma > 999.; sigma -= 1000)
		{
			cout << sigma << ",";
			test(data, normal_distribution<>(data.size() / 2, sigma), true);
		}
	}


/*
	{
		cout << "p,";
		cout << "std::set,std::unordered_set,so::list<count>,so::list<move-to-front>" << endl;

		for(double p = .9; p > .1 + DBL_EPSILON; p -= .1)
		{
			cout << p << ",";
			test(data, geometric_distribution<>(p));
		}

		for(double p = .1; p > .01 - DBL_EPSILON; p -= .01)
		{
			cout << p << ",";
			test(data, geometric_distribution<>(p));
		}
	}
*/

	return 0;
}
