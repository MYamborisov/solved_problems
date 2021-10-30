#pragma once

struct Day {
    double income = 0;
    double spent = 0;
};

Day operator+(Day lhs, Day rhs);