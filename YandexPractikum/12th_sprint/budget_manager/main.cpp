#include "budget_manager.h"
#include "parser.h"

#include <iostream>
#include <string_view>
#include <string>

using namespace std;

void ParseAndProcessQuery(BudgetManager& manager, string_view line) {
    auto [request, date_from, date_to, opt_earn] = ParseRequestLine(line);
    if (request == RequestType::COMPUTE_INCOME) {
        cout << manager.ComputeIncome(date_from, date_to) << endl;
    } else if (request == RequestType::EARN) {
        manager.Earn(date_from, date_to, opt_earn.value());
    } else if (request == RequestType::PAY_TAX) {
        manager.PayTax(date_from, date_to);
    }
}

int ReadNumberOnLine(istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}

int main() {
    BudgetManager manager;

    const int query_count = ReadNumberOnLine(cin);

    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(cin, line);
        ParseAndProcessQuery(manager, line);
    }
}
