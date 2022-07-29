
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
        std::less<int> cmp;
        tlucanti::set_base<int> a(cmp, alloc);
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
        tlucanti::set_base<int> c;
        ASSERT(c.empty(), "basic assign operator test 0");
        c = a;
        ASSERT(c.count(1), "basic copy constructor test 0");
        ASSERT(c.count(2), "basic copy constructor test 1");
        ASSERT(c.count(3), "basic copy constructor test 2");
    }
    {
        std::allocator<int> alloc;
        std::less<int> cmp;
        tlucanti::set_base<int> a(cmp, alloc);
        ASSERT(a.get_allocator() == alloc, "basic get_allocator tes 0");
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
        ASSERT(a.max_size() == std::numeric_limits<tlucanti::set_base<int>::size_type>::max(), "max_size basic test 0");
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
//        typedef tlucanti::pair_base<tlucanti::set_base<int>::iterator, bool> P;
//        tlucanti::set_base<int> a;
//        P p = a.insert(a.begin(), 1);
//        ASSERT(*p.first == 1 && p.second == true, "basic hint-insert test 0");
//        ASSERT(a.size() == 1, "basic insert test 1");
//        p = a.insert(a.begin(), 2);
//        ASSERT(*p.first == 2 && p.second == true, "basic hint-insert test 1");
//        ASSERT(a.size() == 1, "basic insert test 1");
//        p = a.insert(a.begin(), 3);
//        ASSERT(*p.first == 3 && p.second == true, "basic hint-insert test 2");
//        ASSERT(a.size() == 1, "basic insert test 1");
//        p = a.insert(a.begin(), 1);
//        ASSERT(*p.first == 1 && p.second == false, "basic hint-insert test 3");
    }
    {
        std_vec_123(v);
        tlucanti::set_base<int> a;
        a.insert(v.begin(), v.end());
    }
    {
//        tlucanti::set_base<int> a;
//        ASSERT(a.erase(123) == 0, "erase test 0");
//        a.insert(1);
//        a.erase(a.begin());
//        ASSERT(a.empty(), "erase test 1");
//        a.insert(2);
//        ASSERT(a.erase(2) == 1, "erase test 2");
//        ASSERT(a.empty(), "erase test 3");
    }
    {
        set_123(a);
        tlucanti::set_base<int> b;
        a.swap(b);
        ASSERT(a.empty(), "swap basic test 0");
    }
    {
        set_123(a);
        tlucanti::set_base<int> b;
        b.swap(a);
        ASSERT(a.empty(), "swap basic test 1");
    }
    {
        set_123(a);
        ASSERT(a.count(0) == 0, "count basic test 0");
        ASSERT(a.count(1) == 1, "count basic test 1");
        ASSERT(a.count(2) == 1, "count basic test 2");
        ASSERT(a.count(3) == 1, "count basic test 3");
        ASSERT(a.count(4) == 0, "count basic test 4");

//        ASSERT(a.find(0) == a.end(), "find basic test 0");
//        ASSERT(a.find(1) == a.begin(), "find basic test 1");
//        ASSERT(a.find(2) == ++a.begin(), "find basic test 2");
//        ASSERT(*a.find(3) == 3, "find basic test 3");
//        ASSERT(a.find(4) == a.end(), "find basic test 4");
    }
    {
//        typedef tlucanti::set_base<int>::iterator IT;
//        typedef tlucanti::pair_base<IT, IT> P;
//
//        set_123(a);
//        P p = a.equal_range(1);
//        ASSERT(P.first == a.lower_bound(1), "equal_range basic test 0");
//        ASSERT(P.second == a.upper_bound(1), "equal_range basic test 1");
    }
    {
//        set_123(a);
//        ASSERT(*a.lower_bound(0) == 1, "lower_bound basic test 0");
//        ASSERT(*a.lower_bound(1) == 1, "lower_bound basic test 1");
//        ASSERT(*a.lower_bound(2) == 2, "lower_bound basic test 2");
//        ASSERT(*a.lower_bound(3) == 3, "lower_bound basic test 3");
//        ASSERT(a.lower_bound(4) == a.end(), "lower_bound basic test 4");
//
//        ASSERT(*a.upper_bound(0) == 1, "upper_bound basic test 0");
//        ASSERT(*a.upper_bound(1) == 1, "upper_bound basic test 1");
//        ASSERT(*a.upper_bound(2) == 3, "upper_bound basic test 2");
//        ASSERT(a.upper_bound(3) == a.end(), "upper_bound basic test 3");
    }
    {
//        std::less<int> cmp;
//        std::allocator<int> alloc;
//        tlucanti::set_base<int> a(cmp, alloc);
//        ASSERT(cmp == a.key_comp(), "key_comp test 0");
//        ASSERT(cmp == a.value_comp(), "value_comp test 0");
    }
    {
//        tlucanti::set_base<int> a;
//        a.insert(1);
//        a.insert(2);
//        a.insert(3);
//        tlucanti::set_base<int> b;
//        b.insert(1);
//        b.insert(2);
//        b.insert(3);
//        ASSERT(a == b, "comparison test 0");
//        ASSERT(a >= b, "comparison test 0.1");
//        ASSERT(a <= b, "comparison test 0.2");
//        ASSERT(not a != b, "comparison test 0.3");
//        a.insert(4);
//        ASSERT(a > b, "comparison test 1");
//        ASSERT(a >= b, "comparison test 1.1");
//        ASSERT(a != b, "comparison test 1.2");
//        ASSERT(not a < b, "comparison test 1.3");
//        ASSERT(not a <= b, "comparison test 1.4");
//        b.erase(1);
//        ASSERT(b > a, "comparison test 2");
//        ASSERT(b >= a, "comparison test 2.1");
//        ASSERT(a != b, "comparison test 2.2");
//        ASSERT(not b < a, "comaprison test 2.3");
//        ASSERT(not b <= a, "comparison test 2.4");
    }
    result();
}