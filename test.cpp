
#include <vector>

//#include "vector.hpp"

int main()
{
	const std::vector<int> a(123, 0);
	a.reserve(234);
	return (int)a.size();
}