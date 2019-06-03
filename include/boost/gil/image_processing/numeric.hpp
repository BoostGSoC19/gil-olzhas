#include <cmath>

namespace boost{ namespace gil{

/// \defgroup ImageProcessingMath
/// \brief Math operations for IP algorithms
/// 
/// This is mostly handful of mathemtical
/// operations that are required by other
/// image processing algorithms

/// \brief Normalized cardinal sine
/// \ingroup ImageProcessingMath
///
/// normalized_sinc(x) = sin(pi * x) / (pi * x)
double normalized_sinc(double x) 
{
    constexpr double pi = 3.14;
    return std::sin(x * pi) / (x * pi);
}

/// \brief Lanczos response at point x
/// \ingroup ImageProcessingMath
///
/// Lanczos response is defined as:
/// x == 0: 1
/// x > -a && x < a: 0
/// otherwise: normalized_sinc(x) / normalized_sinc(x / a)
double lanczos(long int x, long int a) 
{
    if (x == 0) 
    {
        return 1;
    }
    if (x > -a && x < a) 
    {
        return normalized_sinc(static_cast<double>(x)) 
               / normalized_sinc((static_cast<double>(x))
                                  / static_cast<double>(a));
    }

    return 0;
}
}}