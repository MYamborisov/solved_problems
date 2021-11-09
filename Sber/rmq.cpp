#include "rmq_tests.h"

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

struct Num {
    size_t pos;
    int value;
};

void Solution(const vector<int>& numbers, istream& in, ostream& out) {
    string cmd;
    int L = 0, R = 0;
    deque<Num> possible_max = {{0, numbers[0]}};
    while (in >> cmd) {
        if (cmd == "L") {
            if (possible_max.front().pos < ++L) {
                possible_max.pop_front();
            }
        } else if (cmd == "R") {
            Num new_val;
            new_val.value = numbers[++R];
            new_val.pos = R;
            auto it = (upper_bound(possible_max.rbegin(), possible_max.rend(), new_val, [](const Num& lhs, const Num& rhs){
                return lhs.value < rhs.value;
            }));
            int dist =  distance(possible_max.rbegin(), it);
            possible_max.erase(possible_max.end() - dist, possible_max.end());
            possible_max.push_back(new_val);
        }
        out << possible_max.front().value << endl;
    }
}

int main() {
    Tests();
    int n;
    cin >> n;
    vector<int> numbers(n);
    for (int i = 0; i < n; ++i) {
        cin >> numbers[i];
    }
    Solution(numbers, cin, cout);
}
