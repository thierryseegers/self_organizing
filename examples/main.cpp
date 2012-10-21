#include "self_organizing.h"

#include <algorithm>
#include <iostream>
#include <ratio>

using namespace std;

int main()
{
	namespace so = self_organizing;

	int data[] = {0, 1, 2, 3, 4, 5, 6, 7};

//	so::list<int, so::find_policy::transpose, so::insertion_policy::insert_back> soci;
//	so::list<int, so::find_policy::count, so::insertion_policy::count> soci;
	so::vector<int, so::find_policy::count, so::insertion_policy::count> soci;

	soci.insert(begin(data), end(data));

	copy(soci.begin(), soci.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	soci.find(5);
	soci.find(3);
	soci.find(5);
	soci.find(6);
	soci.find(4);
	soci.find(6);
	soci.find(5);
	soci.find(0);
	soci.find(3);
	soci.find(5);
	soci.find(6);
	soci.find(4);

	copy(soci.begin(), soci.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	soci.erase(soci.begin());
	copy(soci.begin(), soci.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	soci.erase(prev(soci.end()));
	copy(soci.begin(), soci.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

//	so::list<int, so::find_policy::count, so::insertion_policy::insert_back> socici;

//	so::list<int, so::find_policy::move_to_front, so::insertion_policy::count> socimc;


	return 0;
}