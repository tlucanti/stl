
#include <iostream>
#include "color.hpp"
#include <string>

TLU_NAMESPACE_BEGIN

class Benchmark
{
public:

    Benchmark(const std::string &container, int _len=70) :
        len(_len),
        Gr(TermColor(50, 200, 50, false)),
        line(_string_mul("─", len - 2)),
        upper_box("╭" + line + "╮"),
        mid_line("├" + line + "┤"),
        lower_box("╰" + line + "╯"),
        vert("│")
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
            std::cout << ' ' << Gr[mid_line] << std::endl;
        std::cout << ' ' << Gr[vert] <<
                  _string_middle(len - 2, static_cast<int>(func.size()), Y[func]) <<
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
        std::cout << ' ' << Gr[upper_box] << std::endl;
        std::cout << ' ' << Gr[vert] <<
                  _string_middle(len - 2, static_cast<int>(35 + container.size()),
                                Y["Benchmark speed test for "] +
                                P[container] + Y[" container"]) <<
                  Gr[vert] << std::endl;
        std::cout << ' ' << Gr[lower_box] << std::endl;

        std::cout << ' ' << Gr[upper_box] << std::endl;
    }

    void _finish()
    {
        std::cout << ' ' << Gr[lower_box] << std::endl;
    }

    void _result(const std::string &test_size, double ft_time, double std_time)
    {
        double div = ft_time / std_time;
        div = 1 - 1 / (div + 1);
        GradientN gradient(GradientN(Cyan, Green), GradientN(Orange, Red));
        ProgressBar progress(len - 26 - 13);
        if (ft_time >= std_time * 20)
            div = 1;
        TermColor ft_col = Cyan;
        TermColor std_col = Red;
        if (ft_time > std_time)
            std::swap(ft_col, std_col);

        std::cout << ' ' << Gr[vert] << _string_mul(" ", len - 2) << Gr[vert] << std::endl;
        std::string left;
        std::string right;
        {
            std::stringstream _left;
            std::stringstream _right;
            _left << ' ' << Gr[vert] << ' '
                   << test_size
                   << ft_col["ft"] << ' '
                   << _time_gen(ft_time) << ' ';
            _right << Reset << _time_gen(std_time) << std_col[" std"]
                      << ' ' << Gr[vert] << '\r';
            left = _left.str();
            right = _right.str();
        }

        int anim_frames = 40;
        double anim_step = div / anim_frames;
        for (int i=0; i < anim_frames; ++i)
        {
            double anim_div = i * anim_step;
            std::string s;
            s += left;
            s += gradient[anim_div].str();
            s += progress[anim_div];
            s += right;
            write(1, s.c_str(), s.size());
            usleep(8000);
        }
        std::cout << std::endl;
    }

    std::string _string_middle(int width, int str_size, const std::string &str)
    {
        int delta = width - str_size;
        int left_space = delta / 2;
        int right_space = delta / 2 + delta % 2;
        return _string_mul(" ", left_space) + str + _string_mul(" ", right_space);
    }

    std::string _string_mul(const std::string &str, int n)
    {
        std::string out;
        while (n-- > 0)
            out += str;
        return out;
    }

    std::string _time_gen(double time)
    {
        std::stringstream ss;
        if (time < 1)
            time = std::round(time * 1000) / 1000.;
        else
            time = std::round(time * 100) / 100;
        ss << time << "s";
        std::string ret = ss.str();
        return Purple + ret + _string_mul(" ", static_cast<int>(6 - ret.size()));
    }

    int len;
    TermColor Gr;
    std::string line;
    std::string upper_box;
    std::string mid_line;
    std::string lower_box;
    std::string vert;
};

TLU_NAMESPACE_END
