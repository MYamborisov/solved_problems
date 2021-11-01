#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;
using filesystem::path;

enum FileExtensions {
    JS,
    CMD,
    EXE
};

struct ScanStatistics {
    unsigned processed = 0;
    unsigned js_detects = 0;
    unsigned cmd_detects = 0;
    unsigned exe_detects = 0;
    unsigned errors = 0;
};

void ScanFile(const path& file_path, ScanStatistics& scan_statistics) {
    std::ifstream input(file_path, std::ios::in | std::ios::binary);
    if (!input || !file_path.has_extension()) { //  file opening error or file has no extension
        ++scan_statistics.errors;
        return;
    }
    FileExtensions extension;
    if (file_path.extension() == ".js"sv) {
        extension = FileExtensions::JS;
    } else if (file_path.extension() == ".cmd"sv || file_path.extension() == ".bat"sv) {

    } else if (file_path.extension() == ".exe"sv || file_path.extension() == ".dll"sv) {

    }
    string suspicious_string, line;
    while (getline(input, line)) {
        if (line.find(suspicious_string) != string::npos) {

            return;
        }
    }
}

void ScanFolder(const path& suspicious_folder, ostream& output) {
    error_code error;
    auto status = filesystem::status(suspicious_folder, error);
    if (error) {
        output << "Specified directory status error"sv << endl;
        return;
    }
    if (status.type() != filesystem::file_type::directory) {
        output << "Specified path is not a directory"sv << endl;
        return;
    }
    ScanStatistics scan_statistics;
    for (const auto& file : filesystem::directory_iterator(suspicious_folder)) {
        ScanFile(file.path(), scan_statistics);
        ++scan_statistics.processed;
    }
}

int main() {

}
