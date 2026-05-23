#include "Member.h"
#include "LibraryException.h"
#include <stdexcept>

int Member::nextId = 1000;

Member::Member()
    : memberId(nextId++), name("Unknown"), email(""), phone(""),
      activeLoans(0), fineOwed(0.0) {}

Member::Member(const std::string& n, const std::string& e, const std::string& p)
    : memberId(nextId++), name(n), email(e), phone(p),
      activeLoans(0), fineOwed(0.0)
{
    if (n.empty())
        throw InvalidItemException("Member name cannot be empty.");
}

Member::Member(const Member& other)
    : memberId(nextId++), name(other.name), email(other.email),
      phone(other.phone), activeLoans(other.activeLoans), fineOwed(other.fineOwed) {}

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

int Member::getMemberId() const { return memberId;}
const std::string& Member::getName() const { return name;}
const std::string& Member::getEmail() const { return email;}
const std::string& Member::getPhone() const { return phone;}
int Member::getActiveLoans() const { return activeLoans;}
double Member::getFineOwed() const { return fineOwed;}

void Member::incrementLoans() { ++activeLoans;}
void Member::decrementLoans() { if (activeLoans > 0) --activeLoans;}

void Member::addFine(double amount) { if (amount > 0) fineOwed += amount;}
void Member::payFine(double amount) { fineOwed = std::max(0.0, fineOwed - amount);}

bool Member::canBorrow() const {
    return activeLoans < getMaxLoans() && fineOwed == 0.0;
}

bool Member::operator==(const Member& other) const { return memberId == other.memberId; }
bool Member::operator<(const Member& other) const { return name < other.name; }

std::ostream& operator<<(std::ostream& os, const Member& m) {
    os << "[" << m.getMemberType() << " #" << m.memberId << "] " << m.name
       << "  Email: " << m.email
       << "  Loans: " << m.activeLoans << "/" << m.getMaxLoans()
       << "  Fine: " << m.fineOwed << " RON";
    return os;
}

std::istream& operator>>(std::istream& is, Member& m) {
    std::cout << "  Name  : "; std::getline(is >> std::ws, m.name);
    std::cout << "  Email : "; std::getline(is >> std::ws, m.email);
    std::cout << "  Phone : "; std::getline(is >> std::ws, m.phone);
    return is;
}

StudentMember::StudentMember() : Member() {}

StudentMember::StudentMember(const std::string& name, const std::string& email,
                             const std::string& phone)
    : Member(name, email, phone) {}

StudentMember::StudentMember(const StudentMember& other) : Member(other) {}

StudentMember& StudentMember::operator=(const StudentMember& other) {
    if (this != &other) Member::operator=(other);
    return *this;
}

StudentMember::~StudentMember() = default;

FacultyMember::FacultyMember() : Member(), department("Unknown") {}

FacultyMember::FacultyMember(const std::string& name, const std::string& email,
                             const std::string& phone, const std::string& dept)
    : Member(name, email, phone), department(dept) {}

FacultyMember::FacultyMember(const FacultyMember& other)
    : Member(other), department(other.department) {}

FacultyMember& FacultyMember::operator=(const FacultyMember& other) {
    if (this != &other) {
        Member::operator=(other);
        department = other.department;
    }
    return *this;
}

FacultyMember::~FacultyMember() = default;

const std::string& FacultyMember::getDepartment() const { return department; }

bool sameDepartment(const FacultyMember& a, const FacultyMember& b) {
    return a.getDepartment() == b.getDepartment();
}
