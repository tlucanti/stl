
#include <vector>
#include <set>
#include <iterator>

//#include "vector.hpp"

int main()
{
	const std::vector<int> a(123, 0);
	a > a;
	std::vector<int>::iterator i;
	std::set<int>::iterator it;
	return (int)a.size();
}