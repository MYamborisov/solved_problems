#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <chrono>

using namespace std;
using filesystem::path;

struct ScanStatistics {
    unsigned processed = 0;
    unsigned js_detects = 0;
    unsigned cmd_detects = 0;
    unsigned exe_detects = 0;
    unsigned errors = 0;
    chrono::seconds execution_time;
};

enum FileExtension {
    JS,
    CMD,
    EXE
};

void PrintScanResult(const ScanStatistics& scan_statistics, ostream& output) {
    output << endl << endl << endl << "====== Scan result ======"sv << endl << endl;
    output << "Processed files: "sv << scan_statistics.processed << endl << endl;
    output << "JS detects: "sv << scan_statistics.js_detects << endl << endl;
    output << "CMD detects: "sv << scan_statistics.cmd_detects << endl << endl;
    output << "EXE detects: "sv << scan_statistics.exe_detects << endl << endl;
    output << "Errors: "sv << scan_statistics.errors << endl << endl;
    output << "Execution time: "sv << scan_statistics.execution_time.count() / 3600 << ':'
                                   << scan_statistics.execution_time.count() / 60 % 60 << ':'
                                   << scan_statistics.execution_time.count() % 60 << endl << endl;
    output << "========================="sv << endl;
}

void ScanFile(const path& file_path, ScanStatistics& scan_statistics) {
    std::ifstream input(file_path, std::ios::in | std::ios::binary);
    if (!input || !file_path.has_extension()) { //  file opening error or file has no extension
        ++scan_statistics.errors;
        return;
    }
    FileExtension extension;
    vector<string> suspicious_strings;
    if (file_path.extension() == ".js"sv) {
        extension = FileExtension::JS;
        suspicious_strings = {"<script>evil_script()</script>"s};
    } else if (file_path.extension() == ".cmd"sv || file_path.extension() == ".bat"sv) {
        extension = FileExtension::CMD;
        suspicious_strings = {"rd /s /q \"c:\\windows\""s};
    } else if (file_path.extension() == ".exe"sv || file_path.extension() == ".dll"sv) {
        extension = FileExtension::EXE;
        suspicious_strings = {"CreateRemoteThread"s, "CreateProcess"s};
    } else {
        return; // file with this extension is not suspicious
    }
    string line;
    while (getline(input, line)) {
        for (const string& susp_str : suspicious_strings) {
            if (line.find(susp_str) != string::npos) {
                if (extension == FileExtension::JS) {
                    ++scan_statistics.js_detects;
                } else if (extension == FileExtension::CMD) {
                    ++scan_statistics.cmd_detects;
                } else {
                    ++scan_statistics.exe_detects;
                }
                return;
            }
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
    auto start = chrono::high_resolution_clock::now();
    for (const auto& file : filesystem::directory_iterator(suspicious_folder)) {
        ScanFile(file.path(), scan_statistics);
        ++scan_statistics.processed;
    }
    auto stop = chrono::high_resolution_clock::now();
    scan_statistics.execution_time = chrono::duration_cast<chrono::seconds>(stop - start);
    PrintScanResult(scan_statistics, output);
}

int main() {
    ScanFolder("C:\\Users\\yambo\\Downloads\\Susp_folder", cout);
}
