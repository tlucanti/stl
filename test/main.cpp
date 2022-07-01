
#include <iostream>
#include "type_traits.hpp"

int main()
{
	std::cout << tlucanti::is_integral<char *>::value << std::endl;
	std::cout << tlucanti::is_integral<size_t>::value << std::endl;
}
