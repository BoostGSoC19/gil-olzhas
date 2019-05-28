//#include <boost/gil/image_processing/scaling.hpp>

#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/io/read_image.hpp>
#include <boost/gil/io/write_view.hpp>

#include <boost/gil/image.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/gil/rgb.hpp>
#include <boost/gil/pixel.hpp>

#include <boost/mpl/int.hpp>

#include <cmath>
#include <iostream>

namespace gil = boost::gil;
namespace mpl = boost::mpl;
using image_view = decltype(gil::view(std::declval<gil::rgb8_image_t&>()));
double sinc(double x) {
    constexpr double pi = 3.14;
    return std::sin(x * pi) / (x * pi);
}

double lanczos(long int x, long int a) {
    if (x == 0) {
        return 1;
    }
    if (x > -a && x < a) {
        return sinc(x) / sinc((x * 1.0) / a);
    }

    return 0;
}

void lanczos_at(long int source_x, long int source_y, long int target_x, long int target_y, image_view input_view, image_view output_view, long int a) {
    std::uint8_t result_red_channel = 0;
    std::uint8_t result_green_channel = 0;
    std::uint8_t result_blue_channel = 0;
    gil::rgb8_pixel_t result = gil::rgb8_pixel_t(0, 0, 0);
    for (long int y_i = std::max(source_y - a + 1, 0l); y_i <= std::min(source_y + a, input_view.height() - 1); ++y_i) {
        for (long int x_i = std::max(source_x - a + 1, 0l); x_i <= std::min(source_x + a, input_view.width() - 1); ++x_i) {
            auto red_channel = input_view(source_x, source_y).at(std::integral_constant<int, 0>{});
            auto green_channel = input_view(source_x, source_y).at(std::integral_constant<int, 1>{});
            auto blue_channel = input_view(source_x, source_y).at(std::integral_constant<int, 2>{});
            double lanczos_response = lanczos(source_x - x_i, a) * lanczos(source_y - y_i, a);
            result_red_channel += red_channel * lanczos_response;
            result_green_channel += green_channel * lanczos_response;
            result_blue_channel += blue_channel * lanczos_response;

            //result += input_view.at(x, y) * lanczos(x - x_i, a) * lanczos(y - y_i, a);
        }
    }

    output_view(target_x, target_y) = gil::rgb8_pixel_t(result_red_channel, result_green_channel, result_blue_channel);
}

void scale_lanczos(image_view input_view, image_view output_view, long int a) {
    double scale_x = (output_view.width() * 1.0) / input_view.width();
    double scale_y = (output_view.height() * 1.0) / input_view.height();

    for (long int y = 0; y < output_view.height(); ++y) {
        for (long int x = 0; x < output_view.width(); ++x) {
            lanczos_at(x / scale_x, y / scale_y, x, y, input_view, output_view, a);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <input-filename.png> <output-filename.png>\n";
        throw std::invalid_argument("invalid cli arguments");
    }

    gil::rgb8_image_t input;
    gil::read_image(argv[1], input, boost::gil::image_read_settings<gil::png_tag>{});

    gil::rgb8_image_t output(input.width() / 2, input.height() / 2);
    scale_lanczos(gil::view(input), gil::view(output), 5);
    gil::write_view(argv[2], gil::view(output), gil::image_write_info<gil::jpeg_tag>{});
}
