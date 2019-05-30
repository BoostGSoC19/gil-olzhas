#include <boost/gil/image_view.hpp>
#include <boost/gil/rgb.hpp>
#include <boost/gil/pixel.hpp>
#include <boost/gil/image_processing/numeric.hpp>

namespace boost{
namespace gil{
template <typename ImageView>
void lanczos_at(long int source_x, long int source_y, 
                long int target_x, long int target_y, 
                ImageView input_view, ImageView output_view, long int a) 
{
    using pixel_t = typename std::remove_reference<
                      decltype(std::declval<ImageView>()(0, 0))
                    >::type;
    // C++11 doesn't allow auto in lambdas
    using channel_t = typename std::remove_reference<
                        decltype(
                            std::declval<pixel_t>().at(
                                std::integral_constant<int, 0>{}
                            )
                        )
                       >::type;
    pixel_t result_pixel;
    boost::gil::static_transform(result_pixel, result_pixel, 
                                 [](channel_t x) {return 0;});
    
    for (long int y_i = std::max(source_y - a + 1, 0l); 
         y_i <= std::min(source_y + a, input_view.height() - 1); 
         ++y_i) {
        for (long int x_i = std::max(source_x - a + 1, 0l); 
             x_i <= std::min(source_x + a, input_view.width() - 1); 
             ++x_i) {
            double lanczos_response = boost::gil::lanczos(source_x - x_i, a) 
                                      * boost::gil::lanczos(source_y - y_i, a);
            auto op = [lanczos_response](channel_t prev, channel_t next) 
            {
                return static_cast<channel_t>(prev + next * lanczos_response);
            };
            boost::gil::static_transform(result_pixel, 
                                         input_view(source_x, source_y), 
                                         result_pixel, 
                                         op);
        }
    }

    output_view(target_x, target_y) = result_pixel;
}

template <typename ImageView>
void scale_lanczos(ImageView input_view, ImageView output_view, long int a) 
{
    double scale_x = (static_cast<double>(output_view.width())) 
                     / static_cast<double>(input_view.width());
    double scale_y = (static_cast<double>(output_view.height())) 
                     / static_cast<double>(input_view.height());

    for (long int y = 0; y < output_view.height(); ++y) {
        for (long int x = 0; x < output_view.width(); ++x) {
            boost::gil::lanczos_at(static_cast<long int>(static_cast<double>(x)
                                    / scale_x), 
                                   static_cast<long int>(static_cast<double>(y)
                                    / scale_y), 
                                   x, y, input_view, output_view, a);
        }
    }
}
}
}
