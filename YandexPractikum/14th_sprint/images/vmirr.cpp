#include "ppm_image.h"

#include <algorithm>
#include <iostream>
#include <string_view>

using namespace std;

// реализуйте вертикальное отражение
void VMirrInplace(img_lib::Image& image) {
    for (int y = 0; y < image.GetHeight() / 2; ++y) {
        for (int x = 0; x < image.GetWidth(); ++x) {
            swap(image.GetPixel(x, y).r, image.GetPixel(x, image.GetHeight() - 1 - y).r);
            swap(image.GetPixel(x, y).g, image.GetPixel(x, image.GetHeight() - 1 - y).g);
            swap(image.GetPixel(x, y).b, image.GetPixel(x, image.GetHeight() - 1 - y).b);
        }
    }
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
        return 1;
    }

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    VMirrInplace(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}