#include <boost/gil/image_processing/scaling.hpp>

#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/io/read_image.hpp>
#include <boost/gil/io/write_view.hpp>
#include <boost/gil/image.hpp>


#include <iostream>

int main(int argc, char* argv[]) {
    namespace gil = boost::gil;
    if (argc != 3) {
        std::cerr << "usage: " << argv[0] << " <input-filename.png> <output-filename.png>\n";
        throw std::invalid_argument("invalid cli arguments");
    }

    gil::rgb8_image_t input;
    gil::read_image(argv[1], input, boost::gil::image_read_settings<gil::png_tag>{});

    gil::rgb8_image_t output(input.width() / 2, input.height() / 2);
    scale_lanczos(gil::view(input), gil::view(output), 5l);
    gil::write_view(argv[2], gil::view(output), gil::image_write_info<gil::jpeg_tag>{});
}
