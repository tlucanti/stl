
#include <iostream>
#include <vector>
#include "type_traits.hpp"

int main()
{
    std::vector<int> a(3) ;
	std::cout << tlucanti::is_integral<char *>::value << std::endl;
	std::cout << tlucanti::is_integral<size_t>::value << std::endl;
}
