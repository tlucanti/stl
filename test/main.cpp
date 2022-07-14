
#include <iostream>
#include "color.hpp"
#include <unistd.h>

int main()
{
	tlucanti::ProgressBar pb(50);
	int n = 100;
	tlucanti::TermColor fg(255, 100, 100);
	tlucanti::TermColor bg(200, 200, 255);
    tlucanti::Gradient gr(tlucanti::Red, tlucanti::Cyan);
    auto S = tlucanti::Reset;
	bg.set_bg(true);

	std::cout << fg << "lol" << S << std::endl;
	std::cout << bg.str() << "qwe" << S << std::endl;
	for (int i=0; i < n; ++i)
	{
		double s = static_cast<double> (i) / n;
		// std::cout << s << std::endl;
		std::cout << gr[s] << pb[s] << S << '\r' << std::flush;
		usleep(10000);
	}
	std::cout << std::endl;
}
