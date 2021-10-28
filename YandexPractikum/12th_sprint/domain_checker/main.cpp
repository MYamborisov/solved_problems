#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <cassert>

using namespace std;

class Domain {
public:
    Domain(string_view str) : domain_(str) {
        reverse(domain_.begin(), domain_.end());
        domain_.push_back('.');
    }
    bool IsSubdomain(const Domain& other) const {
        if (domain_.size() < other.domain_.size()) {
            return false;
        }
        auto res = std::mismatch(other.domain_.begin(), other.domain_.end(), domain_.begin());

        return res.first == other.domain_.end();
    }
    string_view GetDomain() const {
        return domain_;
    }

    friend bool operator==(const Domain& left, const Domain& right);
private:
    string domain_;
};

bool operator==(const Domain& left, const Domain& right) {
    return left.domain_ == right.domain_;
}

class DomainChecker {
public:
    template <typename InputIt>
    DomainChecker(InputIt first, InputIt last) : forbidden_domains_(first, last) {
        sort(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs){
            return lhs.GetDomain() < rhs.GetDomain();
        });
        last = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs){
            return rhs.IsSubdomain(lhs);
        });
        forbidden_domains_.erase(last, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) const {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain, [](const Domain& lhs, const Domain& rhs){
            return lhs.GetDomain() < rhs.GetDomain();
        });
        if (it == forbidden_domains_.begin()) {
            return false;
        } else {
            return domain.IsSubdomain(*prev(it));
        }
    }

private:
    vector<Domain> forbidden_domains_;
};

std::vector<Domain> ReadDomains(istream& in, size_t number_of_lines) {
    std::vector<Domain> result;
    for (size_t i = 0; i < number_of_lines; ++i) {
        string line;
        getline(in, line);
        result.emplace_back(move(line));
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

namespace tests {

    void TestDomain() {
        Domain d1("check.ru"s);
        Domain d2("co.check.ru"s);
        assert(d1 == Domain("check.ru"s));
        assert(d1.GetDomain() == "ur.kcehc."s);
        assert(d2 == Domain("co.check.ru"s));
        assert(d2.GetDomain() == "ur.kcehc.oc."s);
        assert(d1.IsSubdomain(d1));
        assert(d2.IsSubdomain(d1));
        assert(!d1.IsSubdomain(d2));
    }

    void TestDomainChecker() {
        const std::vector<Domain> forbidden_domains{ Domain{"gdz.ru"s} };
        DomainChecker domain_checker( forbidden_domains.begin(), forbidden_domains.end() );
        assert(domain_checker.IsForbidden(Domain("math.gdz.ru")));
        assert(domain_checker.IsForbidden(Domain("history.gdz.ru")));
        assert(domain_checker.IsForbidden(Domain("biology.gdz.ru")));
        assert(!domain_checker.IsForbidden(Domain("freegdz.ru")));
    }

    void TestReadDomains() {
        istringstream input( "4\n"
                             "gdz.ru\n"
                             "maps.me\n"
                             "m.gdz.ru\n"
                             "com\n"s );
        const std::vector<Domain> domains = ReadDomains( input, ReadNumberOnLine<size_t>( input ) );
        assert( domains.size() == 4u );
        assert( domains[0] == Domain{ "gdz.ru"s } );
        assert( domains[1] == Domain{ "maps.me"s } );
        assert( domains[2] == Domain{ "m.gdz.ru"s } );
        assert( domains[3] == Domain{ "com"s } );
    }

    void TestCase() {
        istringstream input( "4\n"
                             "gdz.ru\n"
                             "maps.me\n"
                             "m.gdz.ru\n"
                             "com\n"
                             "7\n"
                             "gdz.ru\n"
                             "gdz.com\n"
                             "m.maps.me\n"
                             "alg.m.gdz.ru\n"
                             "maps.com\n"
                             "maps.ru\n"
                             "gdz.ua"s );
        ostringstream out;

        const std::vector<Domain> forbidden_domains = ReadDomains(input, ReadNumberOnLine<size_t>(input));
        DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

        const std::vector<Domain> test_domains = ReadDomains(input, ReadNumberOnLine<size_t>(input));
        for (const Domain& domain : test_domains) {
            out << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
        }
        ostringstream correct_output( "Bad\n"
                              "Bad\n"
                              "Bad\n"
                              "Bad\n"
                              "Bad\n"
                              "Good\n"
                              "Good\n"s );
        assert( out.str() == correct_output.str() );
    }

    void Test() {
        TestDomain();
        TestDomainChecker();
        TestReadDomains();
        TestCase();
    }

} // namespace tests

int main() {
    tests::Test();

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}