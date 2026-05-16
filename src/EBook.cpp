#include "EBook.h"
#include "LibraryException.h"
#include <iostream>

EBook::EBook() : DigitalItem(), pageCount(0) {}

EBook::EBook(const std::string& title, const std::string& author, int year, const std::string& fileFormat, double fileSizeMB, int pageCount) : DigitalItem(title, author, year, fileFormat, fileSizeMB), pageCount(pageCount)
{
    if (pageCount < 0)
        throw InvalidItemException("EBook page count cannot be negative.");
}

EBook::EBook(const EBook& other) : DigitalItem(other), pageCount(other.pageCount) {}

EBook& EBook::operator=(const EBook& other) {
    if (this != &other) {
        DigitalItem::operator=(other);
        pageCount = other.pageCount;
    }
    return *this;
}

EBook::~EBook() = default;

std::string EBook::getType() const { return "EBook"; }

void EBook::displayDetails() const {
    std::cout << *this << "\n"
              << "   Pages: " << pageCount << "\n";
}

int EBook::getPageCount() const { return pageCount; }

std::ostream& operator<<(std::ostream& os, const EBook& e) {
    os << static_cast<const DigitalItem&>(e)
       << "  Pages: " << e.pageCount;
    return os;
}

std::istream& operator>>(std::istream& is, EBook& e) {
    is >> static_cast<DigitalItem&>(e);
    std::cout << "  Pages: "; is >> e.pageCount;
    return is;
}
