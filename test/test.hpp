
#ifndef TEST_HPP
# define TEST_HPP

# include <iostream>
# include <vector>
# include <memory>
# include <sstream>
# include <cstdio>
# include <csignal>
# include <map>
# include <stdexcept>
# include <set>

# define __SAFE_TEST 0

# include "defs.hpp"
# include "color.hpp"

int all = 0;
int grand_total = 0;
int ok = 0;
int grand_total_ok = 0;
std::string current_test = "null";

template <typename func_T>
void run_test(func_T func)
{
# if __SAFE_TEST
    try {
        func();
    } catch (std::exception &e) {
        std::cout << tlucanti::R << "test fall with exception: "
        << tlucanti::Y << e.what() << tlucanti::S << std::endl;
    }
# else
    func();
# endif
}

# undef ASSERT
# define ASSERT(__e, __msg) _DO_ASSERT(__e, __msg, false)
# define ASSERT_THROW(__e, __msg) _DO_ASSERT(__e, __msg, true)

# define _DO_ASSERT(e, msg, do_throw) do { \
    current_test = msg; \
    ++all; \
    ++grand_total; \
    try { \
        if (not (e)) \
        { \
            std::cout << tlucanti::R << "assertion failed: " << tlucanti::Y << (msg) << tlucanti::S << std::endl; \
            if (do_throw) throw std::logic_error("assertation failed"); \
        } \
        else { \
            ++ok; \
            ++grand_total_ok; \
        } \
    } catch (std::exception &ex) { \
        std::cout << tlucanti::R << "task fall with exception: " << tlucanti::Y << ex.what() << tlucanti::S << std::endl; \
        if (do_throw) throw ; \
    } \
} while (false)

# ifdef __DEBUG
#  define vec_ASSERT(__vec, __size, __allocated, __begin, __end, __alloc, __msg) \
    ASSERT((__vec).size() == (__size), (__msg) + std::string(" size() check")); \
    ASSERT((__vec).empty() == !(__size), (__msg) + std::string(" empty() check")); \
    ASSERT((__vec)._allocated == (__allocated), (__msg)  + std::string(" allocated check")); \
    ASSERT((__vec).capacity() == (__allocated), (__msg) + std::string(" .capacity() check")); \
    ASSERT((__vec)._begin == (__begin), (__msg) + std::string(" begin iterator check")); \
    ASSERT((__vec)._end == (__end), (__msg) + std::string(" end iterator check")); \
    ASSERT((__vec)._allocator == (__alloc), (__msg) + std::string(" allocator check")); \
    ASSERT((__vec).get_allocator() == (__alloc), (__msg) + std::string(" .get_allocator() check"))
# else /* no __DEBUG */
#  define vec_ASSERT(__vec, __size, __allocated, __begin, __end, __alloc, __msg) \
    ASSERT((__vec).size() == (__size), (__msg) + std::string(" size() check")); \
    ASSERT((__vec).empty() == !(__size), (__msg) + std::string(" empty() check")); \
    ASSERT((__vec).capacity() == (__allocated), (__msg) + std::string(" .capacity() check")); \
    ASSERT((__vec).get_allocator() == (__alloc), (__msg) + std::string(" .get_allocator() check"))
# endif /* __DEBUG */

# define vec_cmp(__msg, __type, __vec, ...) do { \
    __type __cmp[] = {__VA_ARGS__}; \
    std::ptrdiff_t __size = sizeof(__cmp) / sizeof(__type); \
    std::stringstream __ssvc, __sscm, __ss; \
    std::ptrdiff_t __vec_size = static_cast<std::ptrdiff_t>((__vec).size()); \
	bool __ok = true; \
    __ok = (__vec_size == __size); \
    __ssvc << "["; \
    __sscm << "["; \
    for (std::ptrdiff_t __i=0; __i < __size; ++__i) \
    { \
        if (__i == 0) \
        { \
            __sscm << __cmp[0]; \
        } \
        else \
        { \
            __sscm << ", " << __cmp[__i]; \
        } \
        if (__ok && (__vec.data())[__i] != __cmp[__i]) \
        { \
            __ok = false; \
        } \
    } \
	for (std::ptrdiff_t __i=0; __i < __vec_size; ++__i) \
    { \
        if (__i == 0) \
        { \
            __ssvc << (__vec)[0]; \
        } \
        else \
        { \
            __ssvc << ", " << (__vec.data())[__i]; \
        } \
        if ((__vec.data())[__i] != __cmp[__i]) \
        { \
            __ok = false; \
        } \
    } \
	__ssvc << "]"; \
    __sscm << "]"; \
    if (not __ok) \
    { \
        __ss << "vector compare: (expected) " << __sscm.str() << " != (got) " << __ssvc.str() << ": "; \
    } \
    ASSERT(__ok, __ss.str() + (__msg)); \
} while (false)

# define make_vec(__name, __type, ...) \
    __type __cmp[] = {__VA_ARGS__}; \
    std::ptrdiff_t __size = sizeof(__cmp) / sizeof(__type); \
    ft::vector<__type> __name(__cmp, __cmp + __size)

# define make_std_vec(__name, __type, ...) \
    std::vector<__type> __name; \
    { \
        __type __cmp[] = {__VA_ARGS__}; \
        std::ptrdiff_t __size = sizeof(__cmp) / sizeof(__type); \
        __name.assign(__cmp, __cmp + __size); \
    }

template <typename T>
void __to_trash(T &_) {(void)_;}

# define error_test(expr, message) do { \
    ++all; \
    bool __err = true; \
    try { \
        __to_trash(expr); \
        __err = false; \
    } catch (std::exception &) {} \
    if (not __err) \
        std::cout << message ": didnt raise exception\n"; \
    else ++ok; \
} while (false)

void start(const char *msg) {
    all = 0;
    ok = 0;
    std::cout << tlucanti::P << "** results for " << msg << "**" << tlucanti::S << "\n";
}
void result() {
    if (ok == all)
        std::cout << tlucanti::G << "[ OK ] all " << tlucanti::C << all << tlucanti::G << " tests passed " << tlucanti::S << "\n\n";
    else
        std::cout << tlucanti::R << "[FAIL] " << tlucanti::C << ok << " / " << all << tlucanti::R << " tests passed\n\n";
}

void final() {
    const char *status;
    tlucanti::TermColor c;
    if (grand_total_ok == grand_total) {
        c = tlucanti::G;
        status = "       SUCCESS      ";
    } else if (grand_total_ok >= grand_total * 0.9) {
        c = tlucanti::Y;
        status = "        FAIL        ";
    } else {
        c = tlucanti::R;
        status = "      YOU SUCK      ";
    }
    std::cout << c << "╭────────────────────╮" << tlucanti::S << "\n";
    std::cout << c << "│" << status << "│"<< tlucanti::S << "\n";
    std::cout << c << "│ " << "TOTAL TESTS PASSED " << c << "│" << tlucanti::S << "\n";

    printf("%s│     ", c.str().c_str());
    printf("%s", tlucanti::C.str().c_str());
    printf("%3d / %3d %s     │", grand_total_ok, grand_total, c.str().c_str());
    printf("%s", tlucanti::C.str().c_str());
    printf("\n");
    std::cout << c << "╰────────────────────╯" << tlucanti::S << "\n";
}

void sigsegv_catcher(UNUSED(int sig))
{
#if __SAFE_TEST
    throw std::runtime_error("test fall with SIGSEGV");
#else
    std::cout << tlucanti::R["test fall with SIGSEGV\n"];
    exit(1);
#endif
}

void sigill_cathcer(UNUSED(int sig))
{
#if __SAFE_TEST
    throw std::runtime_error("test fall with SIGILL");
#else
    std::cout << tlucanti::R["test fall with SIGILL\n"];
    exit(1);
#endif
}

void sigabrt_catcher(UNUSED(int sig))
{
#if __SAFE_TEST
    throw std::runtime_error("test fall with SIGABRT");
#else
    std::cout << tlucanti::R["test fall with SIGABRT\n"];
    exit(1);
#endif
}

# define vec_123(__vec) ft::vector<int> __vec(3); (__vec)[0] = 1; (__vec)[1] = 2; (__vec)[2] = 3
# define vec_111(__vec) ft::vector<int> __vec(3); (__vec)[0] = 123; (__vec)[1] = 456; (__vec)[2] = 789
# define std_vec_123(__vec) std::vector<int> __vec(3); (__vec)[0] = 1; (__vec)[1] = 2; (__vec)[2] = 3
# define std_vec_111(__vec) std::vector<int> __vec(3); (__vec)[0] = 123; (__vec)[1] = 456; (__vec)[2] = 789
# define const_vec_123(__vec) vec_123(GLUE2(__vec_non_const_macro__, __vec)); \
    const ft::vector<int> __vec = GLUE2(__vec_non_const_macro__, __vec)
# define const_vec_111(__vec) vec_111(GLUE2(__vec_non_const_macro__, __vec)); \
    const ft::vector<int> __vec = GLUE2(__vec_non_const_macro__, __vec)
# define const_std_vec_123(__vec) std_vec_123(GLUE2(__vec_non_const_macro__, __vec)); \
    const std::vector<int> __vec = GLUE2(__vec_non_const_macro__, __vec)

template<class V>
std::string _vec_to_str(const V &v)
{
    std::stringstream ss;
    for (std::ptrdiff_t i=0; i < static_cast<std::ptrdiff_t>(v.size()); ++i)
    {
        if (i == 0)
        {
            ss << '[' << v.data()[0];
        }
        else
        {
            ss << ", " << v.data()[i];
        }
    }
    ss << ']';
    return ss.str();
}

template<class v1T, class v2T>
void std_vec_cmp(const v1T &expected, const v2T &got, const std::string &msg)
{

    bool _ok = true;
    std::string sexp, sgot;
    sexp = _vec_to_str(expected);
    sgot = _vec_to_str(got);
    std::string outmsg;
    if (static_cast<std::ptrdiff_t>(expected.size()) != static_cast<std::ptrdiff_t>(got.size()))
    {
        _ok = false;
        outmsg = "vector size mismatch: ";
        goto _ASSERT_LABEL;
    }
    for (std::ptrdiff_t i=0; i < static_cast<std::ptrdiff_t>(expected.size()); ++i)
    {
        if (expected.data()[i] != got.data()[i])
        {
            _ok = false;
            outmsg = "vector compare: ";
            goto _ASSERT_LABEL;
        }
    }
    _ASSERT_LABEL:
    ASSERT(_ok, outmsg + "(expected) " + sexp + " != (got) " + sgot + ": " + msg);
}


typedef enum e_colors
{
    _ColStr_Green,
    _ColStr_Cyan,
    _ColStr_Red,
    _ColStr_Blue,
    _ColStr_Purple,
    _ColStr_Black
} colors;

struct ColString
{
    std::string _str;
    std::string _col;
    colors _col_e;
    int _cnt;
    ColString(const std::string &str, colors col, int cnt=1)
    {
        _cnt = cnt;
        _str = str;
        _col_e = col;
        switch (col)
        {
            case _ColStr_Green: _col = tlucanti::G.str().c_str(); break;
            case _ColStr_Cyan: _col = tlucanti::C.str().c_str(); break;
            case _ColStr_Red: _col = tlucanti::R.str().c_str(); break;
            case _ColStr_Blue: _col = tlucanti::B.str().c_str(); break;
            case _ColStr_Purple: _col = tlucanti::P.str().c_str(); break;
            case _ColStr_Black: _col = tlucanti::B.str().c_str(); break;
        }
    }

    ColString operator *(int i)
    {
        return ColString(_str, _col_e, i);
    }

    std::string str() const
    {
        std::stringstream ss;
        ss << _col << _str;
        if (_cnt > 1)
            ss << "(x" << _cnt << ')';
        ss << tlucanti::Y;
        return ss.str();
    }

    bool operator ==(const ColString &cmp) const
    {
        return _col_e == cmp._col_e && _cnt == cmp._cnt;
    }

    bool operator !=(const ColString &cmp) const
    {
        return not (*this == cmp);
    }

    bool cmp(const ColString &cmp) const
    {
        return _str == cmp._str;
    }
};

std::ostream &operator <<(std::ostream &out, const ColString &s)
{
    out << s.str();
    return out;
}

ColString Def("def", _ColStr_Green); // = G + std::string("def") + Y;
ColString Cons("cons", _ColStr_Cyan); // = C + std::string("cons") + Y;
ColString Del("del", _ColStr_Red); // = R + std::string("del") + Y;
ColString Cpy("cpy", _ColStr_Blue); // = C + std::string("cpy") + Y;
ColString Icpy("icpy", _ColStr_Blue); // = B + std::string("icpy") + Y;
ColString Mv("mv", _ColStr_Purple); // = P + std::string("mv") + Y;
ColString Imv("imv", _ColStr_Black); // = K + std::string("imv") + Y;

std::vector<ColString> moves;
std::vector<ColString> moves_std;
void convolve(std::vector<ColString> &v)
{
    if (v.empty())
        return ;
    ColString last = v[0];
    int cnt = 0;
    std::vector<ColString> out;
    for (std::size_t i=0; i < v.size(); ++i)
    {
        if (v[i].cmp(last))
            cnt += v[i]._cnt;
        else
        {
            out.push_back(last * cnt);
            cnt = v[i]._cnt;
            last = v[i];
        }
    }
    out.push_back(last * cnt);
    v.swap(out);
}

struct UserClass
{
    int a;
    int b;
    bool *valid;

    static int c;
    static int total_instances;
    static bool verbose;
    static bool monitoring;

    int my_c;
    UserClass() : a(111), b(222) {
        valid = new(bool);
        *valid = true;
        my_c = ++c;
        ++total_instances;
        if (monitoring) {
            moves.push_back(Def);
            convolve(moves);
        }
        if (verbose)
            std::cout << str() << tlucanti::G << " default" << tlucanti::S << "\n";
    }
    UserClass(int _a, int _b) : a(_a), b(_b) {
        valid = new(bool);
        *valid = true;
        my_c = ++c;
        ++total_instances;
        if (monitoring) {
            moves.push_back(Cons);
            convolve(moves);
        }
        if (verbose)
            std::cout << str() << tlucanti::C << " constructor" << tlucanti::S << "\n";
    }
    ~UserClass() EXCEPT(std::runtime_error) {
        if (!valid)
            throw std::runtime_error("nullptr in `valid` value");
        if (not *valid)
            throw std::runtime_error("invalid class");
        delete valid;
        --total_instances;
        if (monitoring) {
            moves.push_back(Del);
            convolve(moves);
        }
        if (verbose)
            std::cout << str() << tlucanti::R << " destructor" << tlucanti::S << "\n";
    }
    WUR std::string cl() const {
        switch (my_c % 7) {
            case 0: return tlucanti::K.str();
            case 1: return tlucanti::R.str();
            case 2: return tlucanti::G.str();
            case 3: return tlucanti::Y.str();
            case 4: return tlucanti::B.str();
            case 5: return tlucanti::P.str();
            case 6: return tlucanti::C.str();
            default: return tlucanti::W.str();
        }
    }
    UserClass(const UserClass &cpy) : a(cpy.a), b(cpy.b) {
        if (!cpy.valid)
            throw std::runtime_error("nullptr in copy `valid` value");
        if (not *cpy.valid)
            throw std::runtime_error("invalid copy class");
        valid = new(bool);
        *valid = true;
        my_c = ++c;
        ++total_instances;
        if (monitoring) {
            moves.push_back(Cpy);
            convolve(moves);
        }
        if (verbose)
            std::cout << str() << tlucanti::Y << " copy" << tlucanti::S << "\n";
    }
    UserClass &operator =(const UserClass &cpy) {
        if (!cpy.valid)
            throw std::runtime_error("nullptr in copy `valid` value");
        if (not *cpy.valid)
            throw std::runtime_error("invalid copy class");
        if (!valid)
            throw std::runtime_error("nullptr in `valid` value");
        if (not *valid)
            throw std::runtime_error("invalid class");
        a = cpy.a;
        b = cpy.b;
        valid = new(bool);
        *valid = true;
        my_c = cpy.my_c;
        if (monitoring) {
            moves.push_back(Icpy);
            convolve(moves);
        }
        if (verbose)
            std::cout << str() << tlucanti::C << " << copy assign" << tlucanti::S << "\n";
        return *this;
    }

    std::string str()
    {
        std::stringstream ss;
        ss << cl() << '[' << this << '(' << my_c << ")]:";
        return ss.str();
    }
//    UserClass(UserClass &&mv) noexcept : a(mv.a), b(mv.b) {
//        my_c = mv.my_c;
//        valid = mv.valid;
//        mv.valid = nullptr;
//        ++total_instances;
//        if (monitoring) {
//            moves.push_back(Mv);
//            convolve(moves);
//        }
//        if (verbose)
//            std::cout << cl() << "[" << this << "]:" P " move" S "\n";
//    }
//    UserClass &operator =(UserClass &&mv) noexcept {
//        a = mv.a;
//        b = mv.b;
//        valid = mv.valid;
//        mv.valid = nullptr;
//        my_c = mv.my_c;
//        ++total_instances;
//        if (monitoring) {
//            moves.push_back(Imv);
//            convolve(moves);
//        }
//        if (verbose)
//            std::cout << cl() << "[" << this << "]:" K " move assign" S "\n";
//        return *this;
//    }
DELETED_MEMBERS:
//    UserClass(const UserClass &) __DELETE
//    UserClass &operator =(const UserClass &) __DELETE
# if CPP11
//    UserClass(UserClass &&) DELETE
//    UserClass &operator =(UserClass &&) DELETE
# endif /* CPP11 */
};
int UserClass::c = 3;
int UserClass::total_instances = 0;
bool UserClass::verbose = false;
bool UserClass::monitoring = false;

# define vec_cmp_lock(__msg, __type, __vec, ...) \
    UserClass::monitoring = false; \
    vec_cmp(__msg, __type, __vec, __VA_ARGS__); \
    UserClass::monitoring = true

# define make_std_vec_lock(__name, __type, ...) \
    UserClass::monitoring = false; \
    make_std_vec(__name, __type, __VA_ARGS__); \
    UserClass::monitoring = true

# define std_vec_cmp_lock(__v1, __v2, __msg) \
    UserClass::monitoring = false; \
    std_vec_cmp(__v1, __v2, __msg); \
    UserClass::monitoring = true

# define PTR(__x) reinterpret_cast<void *>(__x)
# define SINGLE_OP_INIT \
    int __single_op_list [200]; \
    std::memset(__single_op_list, 0, sizeof(int) * 200); \

# define SINGLE_OPERATION(__op, __label, __num) do { \
    if (__single_op_list[__num] == 0) \
    { \
        __op; \
        __single_op_list[__num] = 1; \
        goto __label; \
    } \
} while (false)

template <typename _Rb_node>
int _rb_tree_height_dfs(_Rb_node *node, int cur=1)
{
    int left = 0;
    int right = 0;
    if (node == nullptr)
        return cur;
    if (node->left)
        left = _rb_tree_height_dfs(node->left, cur + 1);
    if (node->right)
        right = _rb_tree_height_dfs(node->right, cur + 1);
    return std::max(std::max(left, right), cur);
}

template <typename _Rb_node>
void _print_rb_level(_Rb_node *node, int level, std::vector<_Rb_node *> &line, int cur=0)
{
    if (cur == level)
    {
        line.push_back(node);
        return ;
    }
    if (node == nullptr)
    {
        _print_rb_level<_Rb_node>(nullptr, level, line, cur + 1);
        _print_rb_level<_Rb_node>(nullptr, level, line, cur + 1);
    } else {
        _print_rb_level<_Rb_node>(node->left, level, line, cur + 1);
        _print_rb_level<_Rb_node>(node->right, level, line, cur + 1);
    }
}

std::string string_mul(const std::string &str, size_t mul)
{
    std::string ret;
    while (mul-- > 0)
        ret += str;
    return ret;
}

unsigned long long ipow(unsigned long long base, unsigned long long power)
{
    if (power == 0)
        return 1;
    else if (power == 1)
        return base;
    else if (power % 2 == 1)
        return base * ipow(base, power - 1);
    unsigned long long pow_half = ipow(base, power / 2);
    return pow_half * pow_half;
}

#ifndef RB_TREE_H
# define _Rb_Red 0
# define _Rb_Black 1
#endif

extern bool _rb_print_toggle;
template <typename _Rb_node>
void _print_rb_tree(_Rb_node *tree, const char *msg)
{
    if (!_rb_print_toggle)
        return ;
    std::cout << tlucanti::W[msg] << std::endl;
    int h = _rb_tree_height_dfs(tree);
    for (int level=0; level < h; ++level)
    {
        std::vector<_Rb_node *> line;
        _print_rb_level(tree, level, line);
        std::cout << string_mul("    ", ipow(2, static_cast<unsigned long long>(h - level - 1)) - 1);
        for (size_t i=0; i < line.size(); ++i)
        {
            std::string space = string_mul("    ", ipow(2, static_cast<unsigned long long>(h - level)) - 1);
            if (line[i] == nullptr)
            {
                std::cout << " .  " << space;
                continue ;
            }
            tlucanti::TermColor col = tlucanti::Red;
            if (line[i]->color == _Rb_Black)
                col = tlucanti::White;
            std::cout << col << '[' << reinterpret_cast<long>(line[i]->key) << ']' << tlucanti::Reset << space;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <typename _Rb_node>
std::string _print_node(_Rb_node *node)
{
    if (node == nullptr)
        return "(null)";
    std::stringstream ss;
    if (node->color == _Rb_Red)
        ss << tlucanti::Red;
    else
        ss << tlucanti::White;
    ss << '[' << reinterpret_cast<std::size_t>(node->key) << ']'
        << tlucanti::Orange << '(' << node << ')' << tlucanti::Reset;
    return ss.str();
}

template <typename rb_tree>
void print_rb_tree(rb_tree *tree, const std::string &msg)
{
    if (_rb_print_toggle)
    {
        std::cout << "root: " << _print_node(tree->root.node)
            << "; begin: " << _print_node(tree->begin.node)
            << "; end: " << _print_node(tree->end.node) << std::endl;
    }
    _print_rb_tree(tree->root.node, msg.c_str());
}

# define CHECK_EDGE(__root, __dir, __root_key, __dir_key, __root_col, __dir_col, __msg) \
    ASSERT((__root), (__msg) + std::string(" root check")); \
    ASSERT((__root)->__dir, (__msg) + std::string(" dir check")); \
    ASSERT((__root)->key == PTR(__root_key), (__msg) + std::string(" root key check")); \
    ASSERT((__root)->__dir->key == PTR(__dir_key), (__msg) + std::string(" dir key check")); \
    ASSERT((__root)->color == (__root_col), (__msg) + std::string(" root color check")); \
    ASSERT((__root)->__dir->color == (__dir_col), (__msg) + std::string(" dir color check")); \
    ASSERT((__root)->__dir->parent == (__root), (__msg) + std::string(" dir parent check"))

# define _R _Rb_Red
# define _B _Rb_Black

# define RB_TREE_INSERT(__tree, __val) do { \
    rb_node __node = rb_insert(&(__tree), PTR(__val), int_compare, nullptr); \
    ASSERT(rb_get_key(__node) == PTR(__val), "insert return test"); \
} while (false)

# define RB_INSERT(__val) RB_TREE_INSERT(tree, __val)

# define RB_REMOVE(__val) do { \
    rb_node __find = rb_find(&tree, PTR(__val), int_compare); \
    if (__find.node != nullptr) \
        __find = rb_next(__find); \
    rb_node __rm = rb_remove(&tree, PTR(__val), int_compare); \
    if (__find.node != nullptr) \
    { \
        ASSERT(__find.node->key == __rm.node->key, "success remove return test"); \
    } \
    else \
        ASSERT(__rm.node == nullptr, "failed remove return test"); \
} while (false)

# define RB_NOT_FOUND(__val, __msg) do { \
    ASSERT(rb_get_key(rb_find(&tree, PTR(__val), int_compare)) == nullptr, __msg); \
} while (false)

# define FIND_ASSERT(__tree, __val, __msg) do { \
    RB_TREE_INSERT(__tree, __val); \
    ASSERT(rb_get_key(rb_find(&__tree, PTR(__val), int_compare)) == PTR(__val), __msg); \
} while (false)

int random(int start, int stop)
{
    int		ret;
    do {
        ret = rand();
    } while (ret >= RAND_MAX - RAND_MAX % (stop - start + 1));
    return ret % (stop - start + 1) + start;
}

template <typename _Rb_node>
void _rb_dfs_black_height_check(
        _Rb_node *node,
        int cur,
        std::set<int> &height_set
    )
{
    assert(node);

    if (node->color == _Rb_Black)
        ++cur;
    if (node->left)
        _rb_dfs_black_height_check(node->left, cur, height_set);
    else
        height_set.insert(cur);
    if (node->right)
        _rb_dfs_black_height_check(node->right, cur, height_set);
    else
        height_set.insert(cur);
}

template <typename _Rb_node>
void _rb_dfs_parent_check(_Rb_node *node)
{
    assert(node->parent);
    if (node->color == _Rb_Red and node->parent->color == _Rb_Red)
        throw std::logic_error("two red nodes in row");
    if (node->left)
    {
        if (node->left->parent != node)
        {
            std::stringstream ss;
            ss << "child's parent pointer not equal actual parent. node: ("
                << node->key << "), child: (" << node->left->key << ")";
            throw std::logic_error(ss.str());
        }
        _rb_dfs_parent_check(node->left);
    }
    if (node->right)
    {
        if (node->right->parent != node)
        {
            std::stringstream ss;
            ss << "child's parent pointer not equal actual parent. node: ("
               << node->key << "), child: (" << node->right->key << ")";
            throw std::logic_error(ss.str());
        }
        _rb_dfs_parent_check(node->right);
    }
}

template <typename rb_tree>
void check_valid_rb_tree(rb_tree *tree)
{
    assert(tree);
    if (tree->root.node == nullptr)
        return ;
    if (tree->root.node->color == _Rb_Red)
        throw std::logic_error("root is Red");
    std::set<int> depth_map;
    _rb_dfs_black_height_check(tree->root.node, 0, depth_map);
    if (depth_map.size() > 1)
        throw std::logic_error("tree has various black height");
    if (tree->root.node->left)
        _rb_dfs_parent_check(tree->root.node->left);
    if (tree->root.node->right)
        _rb_dfs_parent_check(tree->root.node->right);
}

# define RB_TREE_PRINT_LOCK(__tree, __msg) do { \
    _rb_print_toggle = true; \
    print_rb_tree(&(__tree), __msg); \
    _rb_print_toggle = false; \
} while (false)

# define RB_PRINT_LOCK(__msg) RB_TREE_PRINT_LOCK(tree, __msg)

# define RB_INSERT_PRINT_LOCK(__val) do { \
    rb_insert(&tree, PTR(__val), int_compare); \
    RB_PRINT_LOCK(std::string("added node ") + #__val); \
} while (false)

int int_compare(void *lhs, void *rhs)
{
    return static_cast<int>(
            reinterpret_cast<std::ptrdiff_t>(lhs)
            - reinterpret_cast<std::ptrdiff_t>(rhs)
    );
}

template <typename rb_tree>
void generate_rb_tree(rb_tree *tree, std::vector<int> &_moves, int count, int max, rb_tree *tree2=nullptr)
{
    for (int i=0; i < count; ++i)
    {
        int r = rand() % max + 1;
        _moves.push_back(r);
        rb_insert(tree, PTR(r), int_compare, nullptr);
        if (tree2 != nullptr)
            rb_insert(tree2, PTR(r), int_compare, nullptr);
    }
}

template <typename _Rb_node>
void _dfs_compare(_Rb_node *root, const std::set<int> &std_tree)
{
    if (std_tree.count(static_cast<int>(reinterpret_cast<std::size_t>(root->key))) == 0)
    {
        std::stringstream ss;
        ss << "tree has excess element " << root->key;
        throw std::logic_error(ss.str());
    }
    if (root->left)
        _dfs_compare(root->left, std_tree);
    if (root->right)
        _dfs_compare(root->right, std_tree);
}

template <typename rb_tree>
void compare_trees(rb_tree *tree, const std::set<int> &std_tree)
{
    if (std_tree.empty())
    {
        if (tree->root.node != nullptr)
            throw std::logic_error("rb_tree is not empty");
    }
    for (std::set<int>::iterator i=std_tree.begin(); i != std_tree.end(); ++i)
    {
        if (rb_get_key(rb_find(tree, PTR(*i), int_compare)) == nullptr)
        {
            std::stringstream ss;
            ss << "rb_tree don't have element " << (*i);
            throw std::logic_error(ss.str());
        }
    }
    if (tree->root.node)
        _dfs_compare(tree->root.node, std_tree);
}

# define MAKE_USED(__e) do { if (write(-1, &(__e), 8)) {} } while (false)


# define PAIR_ASSERT(__name, __first, __second, __msg) \
    ASSERT((__name).first == (__first), std::string("constructor first argument check ") + (__msg)); \
    ASSERT((__name).second == (__second), std::string("constructor second argument check ") + (__msg))

# define PAIR_SET_ASSERT(__name, __type1, __type2, __first, __second, __msg) \
    ft::pair<__type1, __type2> __name(__first, __second); \
    PAIR_ASSERT(__name, __first, __second, __msg)

# define MAKE_EMPTY_TREE (rb_tree){{nullptr}, {nullptr}, {nullptr}, 0}

# define __RBCPP_METHODVAL_CMPASSERT(__method, __val, __cmp, __msg) do { \
    tlucanti::rb_tree<int>::rb_node *node = tree.__method(__val); \
    if (PTR(__cmp) == nullptr) \
        ASSERT(node == nullptr, __msg); \
    else \
        ASSERT(PTR(node->get_key()) == PTR(__cmp), __msg); \
} while (false)

# define __RBCPP_METHODVAL_ASSERT(__method, __val, __msg) __RBCPP_METHODVAL_CMPASSERT(__method, __val, __val, __msg)
# define __RBCPP_METHODNODE_ASSERT(__method, __nd, __val, __msg) __RBCPP_METHODVAL_CMPASSERT(__method, __nd, __val, __msg)

# define RBCPP_INSERT_ASSERT(__val, __msg) __RBCPP_METHODVAL_ASSERT(insert, __val, __msg);
# define RBCPP_FIND_ASSERT(__val, __msg) __RBCPP_METHODVAL_ASSERT(find, __val, __msg)
# define RBCPP_REMOVE_ASSERT(__val, __cmp, __msg) __RBCPP_METHODVAL_CMPASSERT(remove, __val, __cmp, __msg)
# define RBCPP_NEXT_ASSERT(__nd, __val, __msg) __RBCPP_METHODNODE_ASSERT(next, __nd, __val, __msg)
# define RBCPP_PREV_ASSERT(__nd, __val, __msg) __RBCPP_METHODNODE_ASSERT(prev, __nd, __val, __msg)
# define RBCPP_LOWERBOUND_ASSERT(__val, __cmp, __msg) __RBCPP_METHODVAL_CMPASSERT(lower_bound, __val, __cmp, __msg)
# define RBCPP_UPPERBOUND_ASSERT(__val, __cmp, __msg) __RBCPP_METHODVAL_CMPASSERT(upper_bound, __val, __cmp, __msg)

# define set_123(__name) ft::set<int> __name; (__name).insert(1), (__name).insert(2), (__name).insert(3)

#endif /* PAIR_HPP */
