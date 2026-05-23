#include "LoanPolicy.h"
#include <sstream>
#include <iomanip>

std::string LoanPolicy::computeDueDate(const std::string& borrowDate) const {
    int year = std::stoi(borrowDate.substr(0, 4));
    int month = std::stoi(borrowDate.substr(5, 2));
    int day = std::stoi(borrowDate.substr(8, 2));

    day += getLoanDurationDays();

    int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

    while (day > daysInMonth[month]) {
        day -= daysInMonth[month];
        ++month;
        if (month > 12) { month = 1; ++year; }
    }

    std::ostringstream oss;
    oss << year << "-"
        << std::setw(2) << std::setfill('0') << month << "-"
        << std::setw(2) << std::setfill('0') << day;
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const LoanPolicy& p) {
    os << p.getPolicyName();
    return os;
}
