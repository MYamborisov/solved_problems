#include "ppm_image.h"

#include <iostream>
#include <string_view>
#include <cmath>

using namespace std;

int Sum(img_lib::Color c) {
    return to_integer<int>(c.r) + to_integer<int>(c.g) + to_integer<int>(c.b);
}

void NegateInplace(img_lib::Image& image) {
    for (int y = 0; y < image.GetHeight(); ++y) {
        for (int x = 0; x < image.GetWidth(); ++x) {
            image.GetPixel(x, y).r = byte(255 - to_integer<int>(image.GetPixel(x, y).r));
            image.GetPixel(x, y).g = byte(255 - to_integer<int>(image.GetPixel(x, y).g));
            image.GetPixel(x, y).b = byte(255 - to_integer<int>(image.GetPixel(x, y).b));
        }
    }
}

void HMirrInplace(img_lib::Image& image) {
    for (int y = 0; y < image.GetHeight(); ++y) {
        for (int x = 0; x < image.GetWidth() / 2; ++x) {
            swap(image.GetPixel(x, y).r, image.GetPixel(image.GetWidth() - 1 - x, y).r);
            swap(image.GetPixel(x, y).g, image.GetPixel(image.GetWidth() - 1 - x, y).g);
            swap(image.GetPixel(x, y).b, image.GetPixel(image.GetWidth() - 1 - x, y).b);
        }
    }
}

void VMirrInplace(img_lib::Image& image) {
    for (int y = 0; y < image.GetHeight() / 2; ++y) {
        for (int x = 0; x < image.GetWidth(); ++x) {
            swap(image.GetPixel(x, y).r, image.GetPixel(x, image.GetHeight() - 1 - y).r);
            swap(image.GetPixel(x, y).g, image.GetPixel(x, image.GetHeight() - 1 - y).g);
            swap(image.GetPixel(x, y).b, image.GetPixel(x, image.GetHeight() - 1 - y).b);
        }
    }
}

img_lib::Image Sobel(const img_lib::Image& image) {
    int w = image.GetWidth();
    int h = image.GetHeight();
    img_lib::Image result(w, h, img_lib::Color::Black());
    for (int y = 1; y < h - 1; ++y) {
        for (int x = 1; x < w - 1; ++x) {
            int tl = Sum(image.GetPixel(x - 1, y - 1));
            int tc = Sum(image.GetPixel(x, y - 1));
            int tr = Sum(image.GetPixel(x + 1, y - 1));

            int bl = Sum(image.GetPixel(x - 1, y + 1));
            int bc = Sum(image.GetPixel(x, y + 1));
            int br = Sum(image.GetPixel(x + 1, y + 1));

            int cl = Sum(image.GetPixel(x - 1, y));
            int cr = Sum(image.GetPixel(x + 1, y));

            int gx = -tl - 2 * tc - tr + bl + 2 * bc + br;
            int gy = -tl - 2 * cl - bl + tr + 2 * cr + br;

            byte temp = static_cast<byte>(clamp<double>(sqrt(gx * gx + gy * gy), 0, 255));

            result.GetPixel(x, y).r = temp;
            result.GetPixel(x, y).g = temp;
            result.GetPixel(x, y).b = temp;
        }
    }

    return result;
}

int main(int argc, const char** argv) {
//    if (argc != 3) {
//        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
//        return 1;
//    }

    auto image = img_lib::LoadPPM("patrik.ppm"s);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    //NegateInplace(image);
    //image = Sobel(image);
    VMirrInplace(image);
    //HMirrInplace(image);

    if (!img_lib::SavePPM("my_patrik_v.ppm"s, image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}
