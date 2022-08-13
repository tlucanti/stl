
#ifndef COLOR_HPP
# define COLOR_HPP

# include <string>
# include <cstring>
# include <stdexcept>
# include <sstream>
# include <stdint.h>
# include "defs.hpp"

TLU_NAMESPACE_BEGIN

class TermColor
{
public:
    TermColor() : _red(255u), _green(255u), _blue(255u), _bold(false), _bg(false), _buff()
    {
        _init();
    }

    TermColor(uint8_t red, uint8_t green, uint8_t blue, bool bold=true, bool bg=false)
        : _red(red), _green(green), _blue(blue), _bold(bold), _bg(bg), _buff()
    {
        _init();
    }

    void set_red(uint8_t redv) UNUSED
    {
        _red = redv;
        _init();
    }

    void set_green(uint8_t greenv) UNUSED
    {
        _green = greenv;
        _init();
    }

    void set_blue(uint8_t bluev) UNUSED
    {
        _blue = bluev;
        _init();
    }

    void set_bold(bool boldv) UNUSED
    {
        _bold = boldv;
        _init();
    }

    void set_bg(bool bgv) UNUSED
    {
        _bg = bgv;
        _init();
    }

    uint8_t red() const { return _red; }
    uint8_t green() const { return _green; }
    uint8_t blue() const { return _blue; }
    bool bold() const UNUSED { return _bold; }
    bool bg() const UNUSED { return _bg; }

    std::string str() const
    {
        return std::string(_buff);
    }

    TermColor operator +(const TermColor &other) const
    {
        return TermColor(
            static_cast<uint8_t>(_red + other._red),
            static_cast<uint8_t>(_green + other._green),
            static_cast<uint8_t>(_blue + other._blue),
            _bold | other._bold
        );
    }

    TermColor operator -(const TermColor &other) const
    {
        return TermColor(
            static_cast<uint8_t>(_red - other._red),
            static_cast<uint8_t>(_green - other._green),
            static_cast<uint8_t>(_blue - other._blue),
            _bold | other._bold
        );
    }

    TermColor operator *(const TermColor &other) const
    {
        return TermColor(
            static_cast<uint8_t>(_red * other._red),
            static_cast<uint8_t>(_green * other._green),
            static_cast<uint8_t>(_blue * other._blue)
        );
    }

    TermColor operator *(double value) const
    {
        if (UNLIKELY(value < 0 or value > 1))
            throw std::runtime_error("value can be only in range from 0 to 1");
        return TermColor(
            static_cast<uint8_t>(_red * value),
            static_cast<uint8_t>(_green * value),
            static_cast<uint8_t>(_blue * value),
            _bold
        );
    }

    std::string operator [](const std::string &str) const
    {
        return this->str() + str + "\033[0m";
    }

#if PRECPP11
    TermColor &operator =(const TermColor &cpy)
    {
        if (this == &cpy)
            return *this;
        _red = cpy._red;
        _green = cpy._green;
        _blue = cpy._blue;
        _bold = cpy._bold;
        _bg = cpy._bg;
        std::memcpy(_buff, cpy._buff, 22);
        return *this;
    }
#endif /* PRECPP11 */

private:
    void _init()
	{
        memcpy(_buff, "\033[38;2;RED;GRE;BLU;2m", 22);
		itoa3(_buff + 7, _red);
		itoa3(_buff + 11, _green);
		itoa3(_buff + 15, _blue);
        if (_bold)
            _buff[19] = '1';
        if (_bg)
            _buff[2] = '4';
	}

    static void itoa3(char *str, uint8_t num)
    {
        str[2] = static_cast<char>(num % 10 + '0');
        num /= 10;
        str[1] = static_cast<char>(num % 10 + '0');
        num /= 10;
        str[0] = static_cast<char>(num % 10 + '0');
    }

    uint8_t _red;
    uint8_t _green;
    uint8_t _blue;
    bool    _bold;
    bool    _bg;
    char    _buff[22];
};

TermColor Red = TermColor(255, 0, 0);
TermColor Orange = TermColor(255, 160, 0);
TermColor Yellow = TermColor(255, 255, 0);
TermColor Green = TermColor(0, 255, 0);
TermColor Cyan = TermColor(0, 255, 255);
TermColor Blue = TermColor(0, 0, 255);
TermColor Purple = TermColor(255, 0, 255);

TermColor White = TermColor(255, 255, 255);
TermColor Black = TermColor(0, 0, 0);
const char Reset[] = "\033[0m";

TermColor R = Red;
TermColor G = Green;
TermColor B = Blue;
TermColor C = Cyan;
TermColor P = Purple;
TermColor Y = Yellow;

TermColor W = White;
TermColor K = Black;
const char S[] = "\033[0m";

std::ostream &operator <<(std::ostream &out, const TermColor &color)
{
    out << color.str();
    return out;
}

std::string operator +(const std::string &str, const TermColor &color)
{
    return str + color.str();
}

std::string operator +(const TermColor &color, const std::string &str)
{
    return color.str() + str;
}

class Gradient
{
public:
    Gradient(const TermColor &start, const TermColor &end) :
        coef(end.red() - start.red(), end.green() - start.green(), end.blue() - start.blue()),
        bias(start.red(), start.green(), start.blue())
    {}

    TermColor operator [](double value) const
    {
        return TermColor(
            static_cast<uint8_t>(coef.red * value + bias.red),
            static_cast<uint8_t>(coef.green * value + bias.green),
            static_cast<uint8_t>(coef.blue * value + bias.blue)
        );
    }

private:
    struct t_color
    {
        t_color(int _red, int _green, int _blue) :
            red(_red), green(_green), blue(_blue) {}

        int red;
        int green;
        int blue;
    };

    t_color coef;
    t_color bias;
};

class GradientN
{
public:
    GradientN(const TermColor &c1, const TermColor &c2)
        : g1(nullptr), g2(nullptr)
    {
        g = new Gradient(c1, c2);
    }

    GradientN(const GradientN &_g1, const GradientN &_g2)
        : g(nullptr)
    {
        g1 = new GradientN(_g1);
        g2 = new GradientN(_g2);
    }

    GradientN(const GradientN &cpy)
        : g(cpy.g), g1(cpy.g1), g2(cpy.g2) {}

    ~GradientN()
    {
//        delete g;
//        delete g1;
//        delete g2;
    }

    TermColor operator[](double val)
    {
        if (g != nullptr)
            return g->operator[](val);
        else if (val < 0.5)
            return g1->operator[](val * 2);
        return g2->operator[](val * 2 - 1);
    }

private:
    Gradient *g;
    GradientN *g1, *g2;
};

class ProgressBar
{
public:
    explicit ProgressBar(int _width) : width(_width) {}

    std::string operator[](double progress) const
    {
        if (UNLIKELY(progress < 0 or progress > 1))
            throw std::runtime_error("value can be only in range from 0 to 1");
        static const char l0[] = " ";
        static const char l1[] = "▏";
        static const char l2[] = "▎";
        static const char l3[] = "▍";
        static const char l4[] = "▌";
        static const char l5[] = "▋";
        static const char l6[] = "▊";
        static const char l7[] = "▉";
        static const char l8[] = "█";

        static const char *loads[] = {l0, l1, l2, l3, l4, l5, l6, l7, l8};

        int len = 9;
        int max_progess = width * len;
        int current_progress = static_cast<int>(progress * max_progess);
        std::stringstream ss;
        int i = 0;
        while (current_progress >= len)
        {
            ss << l8;
            ++i;
            current_progress -= len;
        }
        ss << loads[current_progress];
        ++i;
        for (; i <= width; ++i)
            ss << l0;
        return ss.str();
    }

private:
    int width;
};

TLU_NAMESPACE_END

#endif /* COLOR_HPP */
