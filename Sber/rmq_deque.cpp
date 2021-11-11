#include "rmq_tests.h"

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

void Solution(const vector<int>& numbers, istream& in, ostream& out) {
    string cmd;
    int L = 0, R = 0;
    deque<int> possible_max = {numbers[0]};
    while (in >> cmd) {
        if (cmd == "L") {
            if (possible_max.front() == numbers[L++]) {
                possible_max.pop_front();
            }
        } else if (cmd == "R") {
            int new_val = numbers[++R];
            auto it = find_if(possible_max.rbegin(), possible_max.rend(), [new_val](int value){
                return value > new_val;
            });
            int dist =  distance(possible_max.rbegin(), it);
            possible_max.erase(possible_max.end() - dist, possible_max.end());
            possible_max.push_back(new_val);
        }
        out << possible_max.front() << endl;
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
