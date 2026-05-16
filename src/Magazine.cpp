#include "Magazine.h"
#include "LibraryException.h"
#include <iostream>

Magazine::Magazine() : LibraryItem(), issueNumber(0), category("General") {}

Magazine::Magazine(const std::string& title, const std::string& author, int year, int issueNumber, const std::string& category) : LibraryItem(title, author, year), issueNumber(issueNumber), category(category)
{
    if (issueNumber < 0)
        throw InvalidItemException("Issue number cannot be negative.");
}

Magazine::Magazine(const Magazine& other) : LibraryItem(other), issueNumber(other.issueNumber), category(other.category) {}

Magazine& Magazine::operator=(const Magazine& other) {
    if (this != &other) {
        LibraryItem::operator=(other);
        issueNumber = other.issueNumber;
        category = other.category;
    }
    return *this;
}

Magazine::~Magazine() = default;

std::string Magazine::getType() const { return "Magazine"; }

void Magazine::displayDetails() const {
    std::cout << *this << "\n"
              << "   Issue #" << issueNumber
              << "  Category: " << category << "\n";
}

int Magazine::getIssueNumber() const { return issueNumber; }

const std::string& Magazine::getCategory() const { return category; }

bool Magazine::operator>(const Magazine& other) const {
    return issueNumber > other.issueNumber;
}

std::ostream& operator<<(std::ostream& os, const Magazine& m) {
    os << static_cast<const LibraryItem&>(m)
       << "  Issue #" << m.issueNumber
       << "  Category: " << m.category;
    return os;
}

std::istream& operator>>(std::istream& is, Magazine& m) {
    is >> static_cast<LibraryItem&>(m);
    std::cout << "  Issue # : "; is >> m.issueNumber;
    std::cout << "  Category: "; is >> std::ws; std::getline(is, m.category);
    return is;
}
