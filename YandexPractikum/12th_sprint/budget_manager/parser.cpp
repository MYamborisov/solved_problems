#include "parser.h"

std::tuple<RequestType, Date, Date, std::optional<double>> ParseRequestLine(std::string_view line) {
    std::tuple<RequestType, Date, Date, std::optional<double>> result;
    int first_date_start, second_date_start;
    if (line[0] == 'C') {
        std::get<0>(result) = RequestType::COMPUTE_INCOME;
        first_date_start = line.size() - 21;
        second_date_start = line.size() - 10;
    } else if (line[0] == 'E') {
        std::get<0>(result) = RequestType::EARN;
        first_date_start = 5;
        second_date_start = 16;
        std::get<3>(result) = std::stod(static_cast<std::string>(line.substr(27, line.size() - 1)));
    } else {
        std::get<0>(result) = RequestType::PAY_TAX;
        first_date_start = line.size() - 21;
        second_date_start = line.size() - 10;
    }
    std::get<1>(result) = std::get<1>(result).FromString(line.substr(first_date_start, 10));
    std::get<2>(result) = std::get<2>(result).FromString(line.substr(second_date_start, 10));
    return result;
}
