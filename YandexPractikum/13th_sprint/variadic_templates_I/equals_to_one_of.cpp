#include <cassert>
#include <string>
#include <string_view>

using namespace std::literals;

template <typename T0, typename... Types>
bool EqualsToOneOfImpl(const T0& v0, const Types&... values) {
    return (... || (v0 == values));
}

template <typename... Types>
bool EqualsToOneOf(const Types&... values) {
    return EqualsToOneOfImpl(values...);
}

int main() {
    assert(EqualsToOneOf("hello"sv, "hi"s, "hello"s));
    assert(!EqualsToOneOf(1, 10, 2, 3, 6));
    assert(!EqualsToOneOf(8));
}