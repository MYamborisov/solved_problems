void CheckDateTimeValidity(const DateTime& dt) {

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array<int, 12> month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (dt.year < 1 || dt.year > 9999 ||
        dt.month < 1 || dt.month > 12 ||
        dt.day < 1 || dt.day > month_lengths[dt.month - 1] ||
        dt.hour < 0 || dt.hour > 23 ||
        dt.minute < 0 || dt.minute > 59 ||
        dt.second < 0 || dt.second > 59) {
        throw domain_error("DateTime is not valid"s);
    }
}