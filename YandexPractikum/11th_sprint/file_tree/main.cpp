#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

void PrintTree(ostream& dst, const path& p, const filesystem::file_status& status, int offset) {
    dst << string(offset, ' ') << p.filename().string() << endl;
    if (status.type() == filesystem::file_type::directory) {
        using Pair = pair<path, filesystem::file_status>;
        vector<Pair> vec;
        for (auto dir_entry: filesystem::directory_iterator(p)) {
            error_code err;
            auto status = dir_entry.status();

            if (err) {
                return;
            }
            vec.emplace_back(dir_entry.path(), status);
        }
        sort(vec.begin(), vec.end(), [](Pair lhs, Pair rhs){
            return lhs.first.filename().string() > rhs.first.filename().string();
        });
        for (const auto& [path, status] : vec) {
            PrintTree(dst, path, status, offset + 2);
        }
    }
}

// напишите эту функцию
void PrintTree(ostream& dst, const path& p) {
    error_code err;
    auto status = filesystem::status(p, err);

    if (err) {
        return;
    }
    PrintTree(dst, p, status, 0);
}

int main() {
    error_code err;
    filesystem::remove_all("test_dir", err);
    filesystem::create_directories("test_dir"_p / "a"_p, err);
    filesystem::create_directories("test_dir"_p / "b"_p, err);

    ofstream("test_dir"_p / "b"_p / "f1.txt"_p);
    ofstream("test_dir"_p / "b"_p / "f2.txt"_p);
    ofstream("test_dir"_p / "a"_p / "f3.txt"_p);

    ostringstream out;
    PrintTree(cout, "test_dir"_p);
//    assert(out.str() ==
//           "test_dir\n"
//           "  b\n"
//           "    f2.txt\n"
//           "    f1.txt\n"
//           "  a\n"
//           "    f3.txt\n"sv
//    );
}
