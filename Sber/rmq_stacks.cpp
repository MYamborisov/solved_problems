#include "rmq_tests.h"

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

void Solution(const vector<int>& numbers, istream& in, ostream& out) {
    string cmd;
    int R = 0;
    stack <pair<int, int>> s1, s2;
    s1.emplace(numbers[0], numbers[0]);
    while (in >> cmd) {
        if (cmd == "L") {
            if (s2.empty()) {
                while (!s1.empty()) {
                    int value = s1.top().first;
                    s1.pop();
                    int maximum = s2.empty() ? value : max(value, s2.top().second);
                    s2.emplace(value, maximum);
                }
            }
            s2.pop();
        } else if (cmd == "R") {
            int new_val = numbers[++R];
            int maximum = s1.empty() ? new_val : max(new_val, s1.top().second);
            s1.emplace(new_val, maximum);
        }
        int current_max;
        if (s1.empty() || s2.empty()) {
            current_max = s1.empty() ? s2.top().second : s1.top().second;
        } else {
            current_max = max(s1.top().second, s2.top().second);
        }
        out << current_max << endl;
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
