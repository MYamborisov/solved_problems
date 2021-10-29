#pragma once
#include "date.h"

struct DayEarnings{
    double earnings = 0;
};

class BudgetManager {
public:
    inline static const Date START_DATE{2000, 1, 1};
    inline static const Date END_DATE{2100, 1, 1};

    BudgetManager() {
        earnings_per_day_.resize(START_DATE.ComputeDistance(START_DATE, END_DATE) + 1);
    }

    double ComputeIncome(Date from, Date to) const;

    void Earn(Date from, Date to, double earnings);

    void PayTax(Date from, Date to);

private:
    std::vector<DayEarnings> earnings_per_day_;
};
