#pragma once

#include <string>
#include <array>

// напишите эту функцию
inline bool DecodeRLE(const std::string& src_name, const std::string& dst_name) {
    std::ifstream in_file(src_name, std::ios::in | std::ios::binary);
    if (!in_file) {
        return false;
    }
    std::ofstream out_file(dst_name, std::ios::out | std::ios::binary);
    if (!out_file) {
        return false;
    }
    while (true) {
        unsigned char header;
        auto h = in_file.get();
        if (h == std::istream::traits_type::eof()) {
            break;
        }
        header = static_cast<unsigned char>(h);
        int block_type = (header & 1);
        int data_size = (header >> 1) + 1;
        if (block_type) {
            auto c = in_file.get();
            std::string buffer(data_size, c);
            out_file.write(buffer.data(), buffer.size());
        } else {
            std::string buffer;
            buffer.reserve(128);
            in_file.read(buffer.data(), data_size);
            out_file.write(buffer.data(), in_file.gcount());
        }
    }


    return true;
}
