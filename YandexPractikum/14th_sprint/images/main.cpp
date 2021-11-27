#include "ppm_image.h"

#include <iostream>
#include <string_view>

using namespace std;

void NegateInplace(img_lib::Image& image) {
    for (int y = 0; y < image.GetHeight(); ++y) {
        for (int x = 0; x < image.GetWidth(); ++x) {
            image.GetPixel(x, y).r = byte(255 - to_integer<int>(image.GetPixel(x, y).r));
            image.GetPixel(x, y).g = byte(255 - to_integer<int>(image.GetPixel(x, y).g));
            image.GetPixel(x, y).b = byte(255 - to_integer<int>(image.GetPixel(x, y).b));
        };
    }
}

int main(int argc, const char** argv) {
//    if (argc != 3) {
//        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
//        return 1;
//    }

    auto image = img_lib::LoadPPM("1.kissy++.ppm"s);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    NegateInplace(image);

    if (!img_lib::SavePPM("2.kissy++.ppm"s, image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}
