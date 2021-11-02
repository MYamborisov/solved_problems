#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class RandomContainer {
public:
    void Insert(int val) {
        val_to_index_[val] = values_pool_.size();
        values_pool_.push_back(val);
    }
    void Remove(int val) {
        size_t index = val_to_index_[val];
        size_t last = values_pool_.size() - 1;
        val_to_index_[values_pool_[last]] = index;
        val_to_index_.erase(val);
        swap(values_pool_[index], values_pool_[last]);
        values_pool_.erase(values_pool_.end() - 1);
    }
    bool Has(int val) const {
        return val_to_index_.count(val);
    }
    int GetRand() const {
        uniform_int_distribution<int> distr(0, values_pool_.size() - 1);
        int rand_index = distr(engine_);
        return values_pool_[rand_index];
    }

private:
    vector<int> values_pool_;
    unordered_map<int, size_t> val_to_index_;
    mutable mt19937 engine_;
};

int main() {
    RandomContainer container;
    int query_num = 0;
    cin >> query_num;
    for (int query_id = 0; query_id < query_num; query_id++) {
        string query_type;
        cin >> query_type;
        if (query_type == "INSERT"s) {
            int value = 0;
            cin >> value;
            container.Insert(value);
        } else if (query_type == "REMOVE"s) {
            int value = 0;
            cin >> value;
            container.Remove(value);
        } else if (query_type == "HAS"s) {
            int value = 0;
            cin >> value;
            if (container.Has(value)) {
                cout << "true"s << endl;
            } else {
                cout << "false"s << endl;
            }
        } else if (query_type == "RAND"s) {
            cout << container.GetRand() << endl;
        }
    }
}