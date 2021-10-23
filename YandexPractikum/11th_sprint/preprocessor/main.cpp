#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;
using filesystem::path;

static regex include1("\\s*#\\s*include\\s*\"([^\"]*)\"\\s*");
static regex include2("\\s*#\\s*include\\s*<([^>]*)>\\s*");

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}
bool IncludeParse(std::ifstream& in, std::ofstream& out, const path& current, const vector<path>& include_directories);

bool AfterFindFile(std::ofstream& out, const path& p, bool& flag, const vector<path>& include_directories) {
    flag = true;
    std::ifstream in;
    while (!in.is_open()) {
        //cout << "unknown include file " << p.string() <<" at file " << current.string() << " at line " << cntr << endl;
        //return false;
        in.open(p, std::ios::in | std::ios::binary);
    }
    return IncludeParse(in ,out, p, include_directories);
}

bool IncludeParse(std::ifstream& in, std::ofstream& out, const path& current, const vector<path>& include_directories) {
    string line;
    int cntr = 0;
    while (getline(in, line)) {
        ++cntr;
        smatch m;
        if (regex_match(line, m, include1)) {
            auto dir = current.parent_path();
            path p = string(m[1]);
            bool flag = false;
            for (auto now : filesystem::recursive_directory_iterator(dir)) {
                path pth = (dir / p);
                if (now.path() == pth) {
                    bool aff = AfterFindFile(out, pth, flag, include_directories);
                    if (!aff)
                        return false;
                    break;
                }
            }
            if (!flag) {
                for (auto now_ : include_directories) {
                    if (exists(now_)) {
                        for (auto now: filesystem::recursive_directory_iterator(now_)) {
                            path pth = (now_ / p);
                            if (now.path() == pth) {
                                bool aff = AfterFindFile(out, pth, flag, include_directories);
                                if (!aff)
                                    return false;
                            }
                        }
                    }
                }
            }
            if (!flag) {
                cout << "unknown include file " << p.string() <<" at file " << current.string() << " at line " << cntr << endl;
                return false;
            }
        } else if (regex_match(line, m, include2)) {
            path p = string(m[1]);
            bool flag = false;
            if (!flag) {
                for (auto now_ : include_directories) {
                    for (auto now : filesystem::recursive_directory_iterator(now_)) {
                        path pth = (now_ / p);
                        if (now.path() == pth) {
                            bool aff = AfterFindFile(out, pth, flag, include_directories);
                            if (!aff)
                                return false;
                            break;
                        }
                    }
                }
            }
            if (!flag) {
                cout << "unknown include file " << p.string() << " at file " << current.string()
                     << " at line " << cntr << endl;
                return false;
            }
        } else {
            out << line << endl;
        }
    }
    return true;
}

// напишите эту функцию
bool Preprocess(const path& in_file, const path& out_file, const vector<path>& include_directories) {
    std::ifstream in(in_file, std::ios::in | std::ios::binary);
    if (!in) {
        return false;
    }
    std::ofstream out(out_file, std::ios::out | std::ios::binary);
    if (!out) {
        return false;
    }
    return IncludeParse(in ,out, in_file, include_directories);
}


string GetFileContents(string file) {
    ifstream stream(file);

    // конструируем string по двум итераторам
    return {(istreambuf_iterator<char>(stream)), istreambuf_iterator<char>()};
}

void Test() {
    error_code err;
    filesystem::remove_all("sources"_p, err);
    filesystem::create_directories("sources"_p / "include2"_p / "lib"_p, err);
    filesystem::create_directories("sources"_p / "include1"_p, err);
    filesystem::create_directories("sources"_p / "dir1"_p / "subdir"_p, err);

    {
        ofstream file("sources/a.cpp");
        file << "// this comment before include\n"
                "#include \"dir1/b.h\"\n"
                "// text between b.h and c.h\n"
                "#include \"dir1/d.h\"\n"
                "\n"
                "int SayHello() {\n"
                "    cout << \"hello, world!\" << endl;\n"
                "#   include\"dummy.txt\"\n"
                "}\n"sv;
    }
    {
        ofstream file("sources/dir1/b.h");
        file << "// text from b.h before include\n"
                "#include \"subdir/c.h\"\n"
                "// text from b.h after include"sv;
    }
    {
        ofstream file("sources/dir1/subdir/c.h");
        file << "// text from c.h before include\n"
                "#include <std1.h>\n"
                "// text from c.h after include\n"sv;
    }
    {
        ofstream file("sources/dir1/d.h");
        file << "// text from d.h before include\n"
                "#include \"lib/std2.h\"\n"
                "// text from d.h after include\n"sv;
    }
    {
        ofstream file("sources/include1/std1.h");
        file << "// std1\n"sv;
    }
    {
        ofstream file("sources/include2/lib/std2.h");
        file << "// std2\n"sv;
    }

    assert((!Preprocess("sources"_p / "a.cpp"_p, "sources"_p / "a.in"_p,
                        {"sources"_p / "include1"_p,"sources"_p / "include2"_p})));

    ostringstream test_out;
    test_out << "// this comment before include\n"
                "// text from b.h before include\n"
                "// text from c.h before include\n"
                "// std1\n"
                "// text from c.h after include\n"
                "// text from b.h after include\n"
                "// text between b.h and c.h\n"
                "// text from d.h before include\n"
                "// std2\n"
                "// text from d.h after include\n"
                "\n"
                "int SayHello() {\n"
                "    cout << \"hello, world!\" << endl;\n"sv;

    assert(GetFileContents("sources/a.in"s) == test_out.str());
}


int main() {
    Test();
}
