#include <cmath>

namespace boost{
namespace gil{
double normalized_sinc(double x) 
{
    constexpr double pi = 3.14;
    return std::sin(x * pi) / (x * pi);
}

double lanczos(long int x, long int a) 
{
    if (x == 0) {
        return 1;
    }
    if (x > -a && x < a) {
        return normalized_sinc(static_cast<double>(x)) 
                               / normalized_sinc((static_cast<double>(x))
                / static_cast<double>(a));
    }

    return 0;
}
}
}