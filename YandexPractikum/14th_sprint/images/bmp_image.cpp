#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>
#include <cstdint>

using namespace std;

namespace img_lib {

    PACKED_STRUCT_BEGIN BitmapFileHeader {
        char first = 'B';
        char second = 'M';
        uint32_t size;
        int reserve = 0;
        uint32_t padding = 54;
    }
    PACKED_STRUCT_END

    PACKED_STRUCT_BEGIN BitmapInfoHeader {
        uint32_t size_of_header = 40;
        int width;
        int height;
        uint16_t plane_num = 1;
        uint16_t bit_per_pixel = 24;
        uint32_t compression_type = 0;
        uint32_t bytes_num;
        int h_resolution = 11811;
        int v_resolution = 11811;
        int colors_num = 0;
        int significant_colors = 0x1000000;
    }
    PACKED_STRUCT_END

    // функция вычисления отступа по ширине
    static int GetBMPStride(int w) {
        return 4 * ((w * 3 + 3) / 4);
    }

    // напишите эту функцию
    bool SaveBMP(const Path& file, const Image& image) {

        ofstream out(file, ios::binary);

        const int w = image.GetWidth();
        const int h = image.GetHeight();
        const int padding = GetBMPStride(w);

        BitmapFileHeader file_header;
        BitmapInfoHeader info_header;

        file_header.size = file_header.padding + padding * h;

        info_header.width = w;
        info_header.height = h;
        info_header.bytes_num = padding * h;

        out.write(reinterpret_cast<char *>(&file_header), sizeof(BitmapFileHeader));
        out.write(reinterpret_cast<char *>(&info_header), sizeof(BitmapInfoHeader));

//        out << file_header.signature << file_header.size << file_header.reserve << file_header.padding << info_header.size_of_header
//        << info_header.width << info_header.height << info_header.plane_num << info_header.bit_per_pixel << info_header.compression_type
//        << info_header.bytes_num << info_header.h_resolution << info_header.v_resolution << info_header.colors_num << info_header.significant_colors;

        std::vector<char> buff(padding);

        for (int y = h - 1; y >= 0; --y) {
            const Color* line = image.GetLine(y);
            for (int x = 0; x < w; ++x) {
                buff[x * 3 + 0] = static_cast<char>(line[x].b);
                buff[x * 3 + 1] = static_cast<char>(line[x].g);
                buff[x * 3 + 2] = static_cast<char>(line[x].r);
            }
            out.write(buff.data(), padding);
        }

        return out.good();
    }

    // напишите эту функцию
    Image LoadBMP(const Path& file) {
        // открываем поток с флагом ios::binary
        // поскольку будем читать данные в двоичном формате
        ifstream ifs(file, ios::binary);

        BitmapFileHeader file_header;
        BitmapInfoHeader info_header;

        ifs.read(reinterpret_cast<char *>(&file_header), sizeof(BitmapFileHeader));
        ifs.read(reinterpret_cast<char *>(&info_header), sizeof(BitmapInfoHeader));

        if (file_header.first != 'B' || file_header.second != 'M' || file_header.padding != 54 || info_header.size_of_header != 40 || info_header.plane_num != 1 || info_header.bit_per_pixel != 24
        || info_header.compression_type != 0 || info_header.h_resolution != 11811 || info_header.v_resolution != 11811 || info_header.colors_num != 0 || info_header.significant_colors != 0x1000000) {
            return {};
        }

        const int padding = GetBMPStride(info_header.width);

        Image result(info_header.width, info_header.height, Color::Black());
        std::vector<char> buff(padding);

        for (int y = 0; y < info_header.height; ++y) {
            Color* line = result.GetLine(info_header.height - 1 - y);
            ifs.read(buff.data(), padding);

            for (int x = 0; x < info_header.width; ++x) {
                line[x].b = static_cast<byte>(buff[x * 3 + 0]);
                line[x].g = static_cast<byte>(buff[x * 3 + 1]);
                line[x].r = static_cast<byte>(buff[x * 3 + 2]);
            }
        }

        return result;
    }

}  // namespace img_lib