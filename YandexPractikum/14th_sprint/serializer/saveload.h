#include <iostream>
#include <map>
#include <string>
#include <vector>

// Serialization

template <typename T>
void Serialize(T val, std::ostream& out) {
    out.write(reinterpret_cast<const char*>(&val), sizeof(val));
}

inline void Serialize(const std::string& str, std::ostream& out) {
    Serialize(str.size(), out);
    //out.write(str.data(), sizeof(char) * str.size());
    for (const auto& elem : str) {
        Serialize(elem, out);
    }
}

template <typename T>
void Serialize(const std::vector<T>& val, std::ostream& out) {
    Serialize(val.size(), out);
    for (const auto& elem : val) {
        Serialize(elem, out);
    }
}

template <typename T1, typename T2>
void Serialize(const std::map<T1, T2>& val, std::ostream& out) {
    Serialize(val.size(), out);
    for (const auto& [key, value] : val) {
        Serialize(key, out);
        Serialize(value, out);
    }
}

// Deserialization

template <typename T>
void Deserialize(std::istream& in, T& val) {
    in.read(reinterpret_cast<char*>(&val), sizeof(val));
}

inline void Deserialize(std::istream& in, std::string& str) {
    size_t size;
    Deserialize(in, size);
    //in.read(reinterpret_cast<char*>(str.data()), sizeof(char) * size);
    for (size_t i = 0; i < size; ++i) {
        char elem;
        Deserialize(in, elem);
        str.push_back(elem);
    }
}

template <typename T>
void Deserialize(std::istream& in, std::vector<T>& val) {
    size_t size;
    Deserialize(in, size);
    for (size_t i = 0; i < size; ++i) {
        T elem;
        Deserialize(in, elem);
        val.push_back(elem);
    }
}

template <typename T1, typename T2>
void Deserialize(std::istream& in, std::map<T1, T2>& val) {
    size_t size;
    Deserialize(in, size);
    for (size_t i = 0; i < size; ++i) {
        T1 key;
        T2 value;
        Deserialize(in, key);
        Deserialize(in, value);
        val[key] = value;
    }
}

// Поскольку функции шаблонные, их реализации будем писать прямо в h-файле.
// Не забудьте объявить нешаблонные функции как inline.