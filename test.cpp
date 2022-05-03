
#include <vector>
#include "vector.hpp"

void constructor_test();
void assign_test();

int main()
{
    constructor_test();
    assign_test();
}

void constructor_test()
{
    std::vector<int> vec;

    std::allocator<int> alloc;
    tlucanti::vector_base<int> a;
    tlucanti::vector_base<int> b(alloc);
    tlucanti::vector_base<int> c(3);
    tlucanti::vector_base<int> d(3, alloc);
    tlucanti::vector_base<int> e(3, 123);
    tlucanti::vector_base<int> f(3, 123, alloc);
    tlucanti::vector_base<int> g(a.begin(), a.end());
    tlucanti::vector_base<int> h(a.begin(), a.end(), alloc);
    tlucanti::vector_base<int> i(a);
    tlucanti::vector_base<int> j(a, alloc);
    tlucanti::vector_base<int> k(std::move(a));
    tlucanti::vector_base<int> l(std::move(b), alloc);
    tlucanti::vector_base<int> m({1, 2, 3});
    tlucanti::vector_base<int> n({1, 2, 3}, alloc);

    std::vector<int> av;
    std::vector<int> bv(alloc);
    std::vector<int> cv(3);
    std::vector<int> dv(3, alloc);
    std::vector<int> ev(3, 123);
    std::vector<int> fv(3, 123, alloc);
    std::vector<int> gv(vec.begin(), vec.end());
    std::vector<int> hv(vec.begin(), vec.end(), alloc);
    std::vector<int> iv(av);
    std::vector<int> jv(av, alloc);
    std::vector<int> kv(std::move(av));
    std::vector<int> lv(std::move(bv), alloc);
    std::vector<int> mv({1, 2, 3});
    std::vector<int> nv({1, 2, 3}, alloc);
}

void assign_test()
{
    tlucanti::vector_base<int> a;
}