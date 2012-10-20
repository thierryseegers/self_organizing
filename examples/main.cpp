#include "list.h"

#include <algorithm>
#include <iostream>
#include <ratio>

using namespace std;

int main()
{
	namespace so = self_organizing;

	int data[] = {0, 1, 2, 3, 4, 5, 6, 7};

	so::list<int, so::find_policy::transpose, so::insertion_policy::insert_back> soli;
//	so::list<int, so::find_policy::count, so::insertion_policy::count> soli;
	soli.insert(begin(data), end(data));

	copy(soli.begin(), soli.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	soli.find(5);
	soli.find(3);
	soli.find(5);
	soli.find(6);
	soli.find(4);
	soli.find(6);
	soli.find(5);
	soli.find(0);
	soli.find(3);
	soli.find(5);
	soli.find(6);
	soli.find(4);

	copy(soli.begin(), soli.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	soli.erase(soli.begin());
	copy(soli.begin(), soli.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	soli.erase(prev(soli.end()));
	copy(soli.begin(), soli.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

//	so::list<int, so::find_policy::count, so::insertion_policy::insert_back> solici;

//	so::list<int, so::find_policy::move_to_front, so::insertion_policy::count> solimc;


	return 0;
}