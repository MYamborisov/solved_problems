#include "json.h"

using namespace std;

namespace json {

    namespace {

        Node LoadNode(istream& input);

        Node LoadArray(istream& input) {
            Array result;
            char c = '!';
            for (; input >> c && c != ']';) {
                if (c != ',') {
                    input.putback(c);
                }
                result.push_back(LoadNode(input));
            }
            if (c != ']') {
                throw ParsingError("Miss ']' at the end of array");
            }
            return Node(move(result));
        }

        Node LoadNumber(std::istream& input) {
            using namespace std::literals;

            std::string parsed_num;

            // Считывает в parsed_num очередной символ из input
            auto read_char = [&parsed_num, &input] {
                parsed_num += static_cast<char>(input.get());
                if (!input) {
                    throw ParsingError("Failed to read number from stream"s);
                }
            };

            // Считывает одну или более цифр в parsed_num из input
            auto read_digits = [&input, read_char] {
                if (!std::isdigit(input.peek())) {
                    throw ParsingError("A digit is expected"s);
                }
                while (std::isdigit(input.peek())) {
                    read_char();
                }
            };

            if (input.peek() == '-') {
                read_char();
            }
            // Парсим целую часть числа
            if (input.peek() == '0') {
                read_char();
                // После 0 в JSON не могут идти другие цифры
            } else {
                read_digits();
            }

            bool is_int = true;
            // Парсим дробную часть числа
            if (input.peek() == '.') {
                read_char();
                read_digits();
                is_int = false;
            }

            // Парсим экспоненциальную часть числа
            if (int ch = input.peek(); ch == 'e' || ch == 'E') {
                read_char();
                if (ch = input.peek(); ch == '+' || ch == '-') {
                    read_char();
                }
                read_digits();
                is_int = false;
            }

            try {
                if (is_int) {
                    // Сначала пробуем преобразовать строку в int
                    try {
                        return Node(std::stoi(parsed_num));
                    } catch (...) {
                        // В случае неудачи, например, при переполнении,
                        // код ниже попробует преобразовать строку в double
                    }
                }
                return Node(std::stod(parsed_num));
            } catch (...) {
                throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
            }
        }

        Node LoadString(istream& input) {
            static const std::unordered_map<char, char> escape_sequences{
                    {'a','\a'},
                    {'b','\b'},
                    {'f','\f'},
                    {'n','\n'},
                    {'r','\r'},
                    {'t','\t'},
                    {'v','\v'},
                    {'\'','\''},
                    {'\"','\"'},
                    {'\\','\\'}
            };
            string str;
            bool escaped = false;
            char c;
            for (; input.get( c ) && !(!escaped && c == '\"');) {
                if (!escaped) {
                    if (c == '\\') {
                        escaped = true;
                    }
                    else {
                        str += c;
                    }
                }
                else {
                    if (const auto it = escape_sequences.find( c ); it != escape_sequences.end()) {
                        str += it->second;
                    }
                    else {
                        throw ParsingError( "Unknown escape sequence \\"s + std::to_string(c) );
                    }
                    escaped = false;
                }
            }
            if ((escaped) || (c != '\"')) {
                throw ParsingError( "Failed to read string. Could collect only ["s + str + "]"s );
            }
            return Node(move(str));
        }

        Node LoadDict(istream& input) {
            Dict result;
            char c  = '!';
            for (; input >> c && c != '}';) {
                if (c == ',') {
                    input >> c;
                }

                string key = LoadString(input).AsString();
                input >> c;
                result.insert({move(key), LoadNode(input)});
            }
            if (c != '}') {
                throw ParsingError("Miss '}' at the end of dictionary");
            }
            return Node(move(result));
        }

        Node LoadNull(istream& input) {
            std::string result;
            char c;
            for (int i = 0; i < 4; ++i) {
                if(!(input >> c)) {
                    throw ParsingError("Failed to read null");
                }
                result.push_back(c);
            }
            if (result == "null"s) {
                return Node(nullptr);
            } else {
                throw ParsingError("Failed to read null");
            }
        }

        Node LoadBool(istream& input) {
            std::string result;
            char c;
            int n;
            input >> c;
            result.push_back(c);
            if (c == 't')
                n = 3;
            else
                n = 4;
            for (int i = 0; i < n; ++i) {
                input >> c;
                result.push_back(c);
            }
            if (result == "true"s) {
                return Node(true);
            } else if (result == "false"s) {
                return Node(false);
            } else {
                throw ParsingError("Failed to read bool");
            }
        }

        Node LoadNode(istream& input) {
            char c;
            input >> c;

            if (c == '[') {
                return LoadArray(input);
            } else if (c == '{') {
                return LoadDict(input);
            } else if (c == '"') {
                return LoadString(input);
            } else if (c == 'n') {
                input.putback(c);
                return LoadNull(input);
            } else if (c == 't' || c == 'f') {
                input.putback(c);
                return LoadBool(input);
            } else {
                input.putback(c);
                return LoadNumber(input);
            }
        }

    }  // namespace

    Node::Node(bool value) : data_(value) {}
    Node::Node(std::nullptr_t /* value*/) : Node() {}
    Node::Node(int value) : data_(value) {}
    Node::Node(double value) : data_(value) {}
    Node::Node(std::string value) : data_(std::move(value)) {}
    Node::Node(Dict map) : data_(std::move(map)) {}
    Node::Node(Array array) : data_(std::move(array)) {}

    NodeData Node::GetData() const {
        return data_;
    }

    bool Node::IsNull() const {
        if (std::holds_alternative<nullptr_t>(data_)) {
            return true;
        }
        return false;
    }

    bool Node::IsInt() const {
        if (std::holds_alternative<int>(data_)) {
            return true;
        }
        return false;
    }

    bool Node::IsDouble() const {
        if (std::holds_alternative<double>(data_) || IsInt()) {
            return true;
        }
        return false;
    }

    bool Node::IsPureDouble() const {
        if (std::holds_alternative<double>(data_)) {
            return true;
        }
        return false;
    }

    bool Node::IsString() const {
        if (std::holds_alternative<std::string>(data_)) {
            return true;
        }
        return false;
    }

    bool Node::IsBool() const {
        if (std::holds_alternative<bool>(data_)) {
            return true;
        }
        return false;
    }

    bool Node::IsArray() const {
        if (std::holds_alternative<Array>(data_)) {
            return true;
        }
        return false;
    }

    bool Node::IsMap() const {
        if (std::holds_alternative<Dict>(data_)) {
            return true;
        }
        return false;
    }

    const Array &Node::AsArray() const {
        if (std::holds_alternative<Array>(data_)) {
            return get<Array>(data_);
        } else {
            throw logic_error("Try to access to non array value with 'AsArray'");
        }
    }

    const Dict &Node::AsMap() const {
        if (std::holds_alternative<Dict>(data_)) {
            return get<Dict>(data_);
        } else {
            throw logic_error("Try to access to non dict value with 'AsDict'");
        }
    }

    const std::string &Node::AsString() const {
        if (std::holds_alternative<string>(data_)) {
            return get<string>(data_);
        } else {
            throw logic_error("Try to access to non string value with 'AsString'");
        }
    }

    int Node::AsInt() const {
        if (std::holds_alternative<int>(data_)) {
            return get<int>(data_);
        } else {
            throw logic_error("Try to access to non int value with 'AsInt'");
        }
    }

    double Node::AsDouble() const {
        if (std::holds_alternative<double>(data_)) {
            return get<double>(data_);
        } else if (std::holds_alternative<int>(data_)) {
            return get<int>(data_);
        } else {
            throw logic_error("Try to access to non double value with 'AsDouble'");
        }
    }

    bool Node::AsBool() const {
        if (std::holds_alternative<bool>(data_)) {
            return get<bool>(data_);
        } else {
            throw logic_error("Try to access to non bool value with 'AsBool'");
        }
    }

    bool operator==(const Document& left, const Document& right) {
        return left.GetRoot() == right.GetRoot();
    }

    bool operator!=(const Document& left, const Document& right) {
        return !(left == right);
    }

    bool operator==(const Node& left, const Node& right) {
        return left.data_ == right.data_;
    }

    bool operator!=(const Node& left, const Node& right) {
        return !(left == right);
    }

    Document::Document(Node root)
            : root_(move(root)) {
    }

    const Node& Document::GetRoot() const {
        return root_;
    }

    Document Load(istream& input) {
        return Document{LoadNode(input)};
    }

    struct NodePrinter {
        std::ostream& out;
        void operator()(std::nullptr_t) const;
        void operator()(Array array) const;
        void operator()(Dict dict) const;
        void operator()(bool boolean) const;
        void operator()(int integer) const;
        void operator()(double real) const;
        void operator()(std::string str) const;
    };

    void PrintNode(const Node& node, std::ostream& out) {
        visit(NodePrinter{out}, node.GetData());
    }

    void Print(const Document& doc, std::ostream& output) {
        PrintNode(doc.GetRoot(), output);
    }

    void NodePrinter::operator()(std::nullptr_t) const {
        out << "null"s;
    }

    void NodePrinter::operator()(bool boolean) const {
        if (boolean) {
            out << "true"s;
        } else {
            out << "false"s;
        }
    }

    void NodePrinter::operator()(int integer) const {
        out << integer;
    }

    void NodePrinter::operator()(double real) const {
        out << real;
    }

    void NodePrinter::operator()(std::string str) const { //TODO try to optimize string
        std::string with_esc;
        for (auto now : str) {
            if (now == '\"') {
                with_esc.push_back('\\');
                with_esc.push_back('\"');
            } else if (now == '\\') {
                with_esc.push_back('\\');
                with_esc.push_back('\\');
            } else {
                with_esc.push_back(now);
            }
        }
        out << '"' << with_esc << '"';
    }

    void NodePrinter::operator()(Array array) const {
        int size = array.size() - 1;
        out << '[';
        for (const Node& node : array) {
            PrintNode(node, out);
            if (size > 0) {
                out << ',';
                --size;
            }
        }
        out << ']';
    }

    void NodePrinter::operator()(Dict dict) const {
        int size =  dict.size() - 1;
        out << '{';
        for (const auto& [key, node] : dict) {
            out << '"' << key << "\": ";
            PrintNode(node, out);
            if (size > 0) {
                out << ',';
                --size;
            }
        }
        out << '}';
    }

}  // namespace json