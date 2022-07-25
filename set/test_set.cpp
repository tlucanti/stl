
#include "set_base.hpp"
#include "rb_tree.hpp"
#include <iostream>
#include <set>

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
    std::set<int> a;
    a.find(123);
    tlucanti::rb_tree<int, std::less<int> > tree((std::less<int>()));
    tree.insert(123);
    tree.find(123);
    tree.remove(123);

}