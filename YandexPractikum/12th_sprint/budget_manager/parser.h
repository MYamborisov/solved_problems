#pragma once
#include "date.h"
#include <utility>
#include <optional>

enum RequestType {
    COMPUTE_INCOME,
    EARN,
    PAY_TAX
};

std::tuple<RequestType, Date, Date, std::optional<double>> ParseRequestLine(std::string_view line);