#include "budget_manager.h"

#include <numeric>

double BudgetManager::ComputeIncome(Date from, Date to) const {
    auto it_start = earnings_per_day_.begin() + from.ComputeDistance(START_DATE ,from);
    auto it_end = it_start + to.ComputeDistance(from ,to) + 1;
    return std::accumulate(it_start, it_end, 0.0, [](double sum, const DayEarnings &current){
        return sum + current.earnings;
    });
}

void BudgetManager::Earn(Date from, Date to, double earnings) {
    double earnings_per_day = earnings / (from.ComputeDistance(from ,to) + 1);
    auto it_start = earnings_per_day_.begin() + from.ComputeDistance(START_DATE ,from);
    auto it_end = it_start + to.ComputeDistance(from ,to) + 1;
    for (auto it = it_start; it != it_end; ++it) {
        it->earnings += earnings_per_day;
    }
}

void BudgetManager::PayTax(Date from, Date to) {
    auto it_start = earnings_per_day_.begin() + from.ComputeDistance(START_DATE ,from);
    auto it_end = it_start + to.ComputeDistance(from ,to) + 1;
    for (auto it = it_start; it != it_end; ++it) {
        it->earnings *= 0.87;
    }
}
