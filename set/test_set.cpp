
#include "set_base.hpp"
#include "rb_tree_iterator.hpp"
#include <set>
#include <iostream>

class UC
{
public:
    UC() {}

    bool operator <(const UC &b) const
    {
        (void)b;
        return true;
    }
};

int main()
{
    tlucanti::set_base<int> a;

    a.insert(123);
    std::cout << "pointer " << a.find(123) << std::endl;
    std::cout << "value " << *static_cast<int *>(a.find(123)) << std::endl;
//    std::cout << "reversed " << rev.next() << std::endl;
//    std::cout << "not reversed " << nrev.next() << std::endl;
}