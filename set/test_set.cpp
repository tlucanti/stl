
#include <iostream>
#include <set>
#include "test.hpp"
#include "rb_tree.hpp"
#include "set_base.hpp"

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

void constructor_test();

int main()
{
    tlucanti::set_base<int> a;
    a.find(123);
    tlucanti::rb_tree<int, std::less<int> > tree((std::less<int>()), std::allocator<int>());
    tree.insert(123);
    tree.find(123);
    tree.remove(123);
}

void set_test()
{
    start("set test");

    {
        tlucanti::set_base<int> a;
        ASSERT(a.empty(), "basic constructor test 0");
    }
    {
        std::allocator<int> alloc;
        tlucanti::set_base<int> a(alloc);
    }
    {
        std::vector<int> stda;
        stda.push_back(1);
        stda.push_back(2);
        stda.push_back(3);
        tlucanti::set_base<int> a(stda.begin(), stda.end());
        ASSERT(a.count(1), "basic iterator constructor test 0");
        ASSERT(a.count(2), "basic iterator constructor test 1");
        ASSERT(a.count(3), "basic iterator constructor test 2");
        tlucanti::set_base<int> b(a);
        ASSERT(b.count(1), "basic copy constructor test 0");
        ASSERT(b.count(2), "basic copy constructor test 1");
        ASSERT(b.count(3), "basic copy constructor test 2");
        tlucant::set_base<int> c;
        ASSERT(c.empty(), "basic assign operator test 0");
        c = a;
        ASSERT(c.count(1), "basic copy constructor test 0");
        ASSERT(c.count(2), "basic copy constructor test 1");
        ASSERT(c.count(3), "basic copy constructor test 2");
    }
    {
        std::allocator<int> alloc;
        tlucanti::set_base<int> a(alloc);
        ASSERT(a.get_allocator() == a, "basic get_allocator tes 0");
    }
    {
        std::vector<int> stda;
        stda.push_back(1);
        stda.push_back(2);
        stda.push_back(3);
        tlucanti::set_base<int> a(stda.begin(), stda.end());
        ASSERT(*a.begin() == 1, "basic iterator test 0");
        ASSERT(*++a.begin() == 2, "basic iterator test 1");
        ASSERT(*++(++a.begin()) == 3, "basic iterator test 2");

        ASSERT(*(a.begin()++) == 1, "advanced iterator test 0");
        ASSERT(*((a.begin()++)++) == 1, "advanced iterator test 1");

        ASSERT(*--a.end() == 3, "basic iterator test 3");
        ASSERT(*--(--a.end()) == 2, "basic iterator test 4");
        ASSERT(*--(--(--a.end())), "basic iterator test 5");

        ASSERT(*((--a.end())++) == 3, "advanced iterator test 2");
        ASSERT(*(((--a.end())++)++) == 3, "advanced iterator test 3");

        ASSERT(*a.rbegin() == 3, "basic reverse iterator test 0");
        ASSERT(*++a.rbegin() == 2, "basic reverse iterator test 1");
        ASSERT(*++(++a.rbegin()) == 1, "basic reverse itertor test 2");

        ASSERT(*(a.rbegin()++) == 3, "advanced reverse iterator test 0");
        ASSERT(*((a.rbegin()++)++), "advanced reverse iterator test 1");

        ASSERT(*--a.rend() == 1, "basic reverse iterator test 3");
        ASSERT(*--(--a.rend()) == 2, "basic reverse iterator test 4");
        ASSERT(*--(--(--a.end())) == 1, "basic reverse iterator test 5");

        ASSERT(*((--a.rend())++) == 1, "advanced reverse iterator test 2");
        ASSERT(*(((--a.rend())++)++) == 1, "advanced reverse iterator test 3");
    }
    {
        tlucanti::set_base<int> a;
        set_123(b);
        ASSERT(a.empty(), "empty test 0");
        ASSERT(b.empty() == false, "empty test 1");
        ASSERT(a.size() == 0, "size test 0");
        ASSERT(b.size() == 3, "size test 0");
    }
    {
        tlucanti::set_base<int> a;
        ASSERT(a.max_size() == std::numeric_limits<int>::max(), "max_size basic test 0");
    }
    {
        set_123(a);
        a.clear();
        ASSERT(a.empty(), "clear basic test 0");
    }
    {
        typedef tlucanti::pair_base<tlucanti::set_base<int>::iterator, bool> P;
        tlucanti::set_base<int> a;
        P p = a.insert(1);
        ASSERT(*p.first == 1 && p.second == true, "basic insert test 0");
        ASSERT(a.size() == 1, "basic insert test 1");
        p = a.insert(2);
        ASSERT(*p.first == 2 && p.second == true, "basic insert test 1");
        ASSERT(a.size() == 1, "basic insert test 1");
        p = a.insert(3);
        ASSERT(*p.first == 3 && p.second == true, "basic insert test 2");
        ASSERT(a.size() == 1, "basic insert test 1");
        p = a.insert(1);
        ASSERT(*p.first == 1 && p.second == false, "basic insert test 3");
    }
    {
        typedef tlucanti::pair_base<tlucanti::set_base<int>::iterator, bool> P;
        tlucanti::set_base<int> a;
        P p = a.insert(1);
        ASSERT(*p.first == 1 && p.second == true, "basic insert test 0");
        ASSERT(a.size() == 1, "basic insert test 1");
        p = a.insert(2);
        ASSERT(*p.first == 2 && p.second == true, "basic insert test 1");
        ASSERT(a.size() == 1, "basic insert test 1");
        p = a.insert(3);
        ASSERT(*p.first == 3 && p.second == true, "basic insert test 2");
        ASSERT(a.size() == 1, "basic insert test 1");
        p = a.insert(1);
        ASSERT(*p.first == 1 && p.second == false, "basic insert test 3");
    }
    {
        std::set<int> a;
        a.insert(a.begin(), 123);
    }

    result();
}