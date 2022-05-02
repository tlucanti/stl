
#include <vector>
#include "vector.hpp"

int main()
{
    std::allocator<int> alloc;

    tlucanti::vector_base<int> a;
    tlucanti::vector_base<int> b(alloc);
    tlucanti::vector_base<int> c(3);
    tlucanti::vector_base<int> d(3, alloc);
    tlucanti::vector_base<int> e(3, 123);


    std::vector<int> av;
    std::vector<int> bv(alloc);
    std::vector<int> cv(3);
//    std::vector<int> dv(3, alloc);
    std::vector<int> ev(3, 123);
    std::vector<int> fv(av.begin(), av.end());
}