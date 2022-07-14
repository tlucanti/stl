
#include <iostream>
#include "color.hpp"
#include <string>

TLU_NAMESPACE_BEGIN

class Benchmark
{
public:

    Benchmark(const std::string &container, int _len=70) : len(_len)
    {
        _start(container);
    }

    ~Benchmark()
    {
        _finish();
    }

    void next_test(const std::string func)
    {
        static bool first_time = true;

        if (not first_time)
            std::cout << G[mid_line] << std::endl;
        std::cout << Gr[vert] <<
                  string_middle(len - 2, func.size(),Y[func]) <<
                  Gr[vert] << std::endl;

        first_time = false;
    }

    void small(double ft_time, double std_time)
    {
        _result(TermColor(100, 200, 0)["small tests : "], ft_time, std_time);
    }

    void medium(double ft_time, double std_time)
    {
        _result(TermColor(200, 200, 0)["medium tests: "], ft_time, std_time);
    }

    void large(double ft_time, double std_time)
    {
        _result(TermColor(200, 100, 0)["large tests : "], ft_time, std_time);
    }

private:
    void _start(const std::string &container)
    {
        std::cout << Gr[upper_box] << std::endl;
        std::cout << Gr[vert] <<
                  string_middle(len - 2, 35 + container.size(),
                                Y["Benchmark speed test for "] +
                                P[container] + Y[" container"]) <<
                  Gr[vert] << std::endl;
        std::cout << Gr[lower_box] << std::endl;

        std::cout << Gr[upper_box] << std::endl;
    }

    void _finish()
    {
        std::cout << Gr[lower_box] << std::endl;
    }

    void _result(const std::string &test_size, double ft_time, double std_time)
    {
        TermColor ft_col, std_col;
        Gradient ft_grad(Red, Green);
        Gradient std_grad(Green, Red);
        ProgressBar pb(len - 26);
        double div = ft_time / std_time;
        if (ft_time >= std_time * 20)
        {
            ft_col = Red;
            std_col = Green;
        }
        else
        {
            ft_col = ft_grad[div / 20];
            std_col = std_grad[div / 20];
        }
        std_col.set_bg(true);


        std::cout << Gr[vert] << ' ' <<
            test_size <<
            Y["ft"] << ' ' <<
            ft_col << std_col <<
            pb[1 - div / 20] << Reset << Y["  std "] <<
            Gr[vert] << std::endl;
    }

    std::string string_middle(int width, int str_size, const std::string &str)
    {
        int delta = width - str_size;
        int left_space = delta / 2;
        int right_space = delta / 2 + delta % 2;
        return string_mul(" ", left_space) + str + string_mul(" ", right_space);
    }

    std::string string_mul(const std::string &str, int n)
    {
        std::string out;
        while (n-- > 0)
            out += str;
        return out;
    }

    int len;
    TermColor Gr =TermColor(50, 200, 50, false);
    std::string line = string_mul("─", len - 2);
    std::string upper_box = "╭" + line + "╮";
    std::string mid_line = "├" + line + "┤";
    std::string lower_box = "╰" + line + "╯";
    std::string vert = "│";
};

TLU_NAMESPACE_END
