#include <iostream>
#include <unordered_map>
#include <deque>
#include <tuple>

using namespace std;

class HotelManager {
public:
    void Book(int64_t time, const string& hotel_name, int64_t client_id, int64_t room_count) {
        int cnt = 0;
        for (auto [hotel, t, client, rooms] : hotel_to_time_to_client_to_rooms) {
            if (t > time - 86400) {
                break;
            } else {
                ++cnt;
                hotel_to_rooms[hotel] -= rooms;
                if (--hotel_to_client_to_books[hotel][client] == 0) {
                    --hotel_to_clients[hotel];
                }
            }
        }
        hotel_to_time_to_client_to_rooms.erase(hotel_to_time_to_client_to_rooms.begin(), hotel_to_time_to_client_to_rooms.begin()+cnt);
        hotel_to_time_to_client_to_rooms.emplace_back(hotel_name, time, client_id, room_count);
        hotel_to_rooms[hotel_name] += room_count;
        if (hotel_to_client_to_books[hotel_name][client_id]++ == 0) {
            ++hotel_to_clients[hotel_name];
        }
    }
    int ComputeClientCount(const string& hotel_name) {
        return hotel_to_clients[hotel_name];
    }
    int ComputeRoomCount(const string& hotel_name) {
        return hotel_to_rooms[hotel_name];
    }

private:
    unordered_map<string, int64_t> hotel_to_rooms;
    unordered_map<string, unordered_map<int64_t, int64_t>> hotel_to_client_to_books;
    unordered_map<string, int64_t> hotel_to_clients;
    deque<tuple<string, int64_t, int64_t, int64_t>> hotel_to_time_to_client_to_rooms;
};

int main() {
    HotelManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id, room_count;
            cin >> client_id >> room_count;
            manager.Book(time, hotel_name, client_id, room_count);
        } else {
            string hotel_name;
            cin >> hotel_name;
            if (query_type == "CLIENTS") {
                cout << manager.ComputeClientCount(hotel_name) << "\n";
            } else if (query_type == "ROOMS") {
                cout << manager.ComputeRoomCount(hotel_name) << "\n";
            }
        }
    }

    return 0;
}