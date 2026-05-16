#include "Member.h"
#include <stdexcept>

int Member::nextId = 1000;

Member::Member() : memberId(nextId++), name("Unknown"), email(""), phone(""), activeLoans(0), fineOwed(0.0) {}

Member::Member(const std::string& name, const std::string& email, const std::string& phone) : memberId(nextId++), name(name), email(email), phone(phone), activeLoans(0), fineOwed(0.0)
{
    if (name.empty())
        throw std::invalid_argument("Member name cannot be empty.");
}

Member::Member(const Member& other) : memberId(other.memberId), name(other.name), email(other.email), phone(other.phone), activeLoans(other.activeLoans), fineOwed(other.fineOwed) {}

Member& Member::operator=(const Member& other) {
    if (this != &other) {
        name = other.name;
        email = other.email;
        phone = other.phone;
        activeLoans = other.activeLoans;
        fineOwed = other.fineOwed;
    }
    return *this;
}

Member::~Member() = default;

int Member::getMemberId() const { return memberId; }
const std::string& Member::getName() const { return name; }
const std::string& Member::getEmail() const { return email; }
const std::string& Member::getPhone() const { return phone; }
int Member::getActiveLoans() const { return activeLoans; }
double Member::getFineOwed() const { return fineOwed; }

void Member::incrementLoans() { ++activeLoans; }
void Member::decrementLoans() {
    if (activeLoans > 0) --activeLoans;
}

void Member::addFine(double amount) {
    if (amount > 0) fineOwed += amount;
}

void Member::payFine(double amount) {
    fineOwed -= amount;
    if (fineOwed < 0) fineOwed = 0.0;
}

bool Member::operator==(const Member& other) const {
    return memberId == other.memberId;
}

bool Member::operator<(const Member& other) const {
    return name < other.name;
}

std::ostream& operator<<(std::ostream& os, const Member& m) {
    os << "[Member #" << m.memberId << "] " << m.name
       << "  Email: " << m.email
       << "  Phone: " << m.phone
       << "  Active loans: " << m.activeLoans
       << "  Fine: " << m.fineOwed << " RON";
    return os;
}

std::istream& operator>>(std::istream& is, Member& m) {
    std::cout << "  Name  : "; std::getline(is >> std::ws, m.name);
    std::cout << "  Email : "; std::getline(is >> std::ws, m.email);
    std::cout << "  Phone : "; std::getline(is >> std::ws, m.phone);
    return is;
}
