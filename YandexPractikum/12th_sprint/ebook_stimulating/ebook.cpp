#include <iostream>
#include <array>
#include <unordered_map>

using namespace std;

constexpr int MAX_PAGES = 1000;

class ReadersDataBase {
public:
    void Read(int user, int page) {
        for (int i = id_to_pages_[user] + 1; i <= page; ++i) {
            ++read_at_least_[i];
        }
        id_to_pages_[user] = page;
    }
    double Cheer(int user) const {
        if (id_to_pages_.count(user) == 0) {
            return 0;
        }
        if (id_to_pages_.size() == 1) {
            return 1;
        }
        return (id_to_pages_.size() - read_at_least_[id_to_pages_.at(user)]) / static_cast<double>(id_to_pages_.size() - 1);
    }
private:
    array<int, MAX_PAGES + 1> read_at_least_ = {};
    unordered_map<int, int> id_to_pages_;
};

void ProcessRequests(int num_of_requests, ostream& out) {
    string command;
    ReadersDataBase db;
    for (int i = 0; i < num_of_requests; ++i) {
        cin >> command;
        if (command == "READ"s) {
            int user, page;
            cin >> user >> page;
            db.Read(user, page);
        } else if (command == "CHEER"s) {
            int user;
            cin >> user;
            out << db.Cheer(user) << endl;
        }
    }
}

int main() {
    int num_of_requests;
    cin >> num_of_requests;
    ProcessRequests(num_of_requests, cout);
}
