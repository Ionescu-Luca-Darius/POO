#include "Book.h"
#include "LibraryException.h"
#include <iostream>

PhysicalLoanPolicy Book::policy;

Book::Book() : LibraryItem(), isbn("000-0000000000"), pageCount(0) {}

Book::Book(const std::string& title, const std::string& author,
           int year, const std::string& isbn, int pageCount)
    : LibraryItem(title, author, year), isbn(isbn), pageCount(pageCount)
{
    if (pageCount < 0)
        throw InvalidItemException("Page count cannot be negative.");
}

Book::Book(const Book& other)
    : LibraryItem(other), isbn(other.isbn), pageCount(other.pageCount) {}

Book& Book::operator=(const Book& other) {
    if (this != &other) {
        LibraryItem::operator=(other);
        isbn = other.isbn;
        pageCount = other.pageCount;
    }
    return *this;
}

Book::~Book() = default;

std::string Book::getType() const { return "Book"; }
const LoanPolicy& Book::getLoanPolicy() const { return policy; }

void Book::displayDetails() const {
    std::cout << *this << "\n"
              << "   ISBN: " << isbn << "  Pages: " << pageCount
              << "  Policy: " << policy.getPolicyName() << "\n";
}

const std::string& Book::getIsbn() const { return isbn;}
int Book::getPageCount() const { return pageCount;}

bool Book::operator>(const Book& other) const {
    return pageCount > other.pageCount;
}

std::ostream& operator<<(std::ostream& os, const Book& b) {
    os << static_cast<const LibraryItem&>(b)
       << "  ISBN: " << b.isbn << "  Pages: " << b.pageCount;
    return os;
}

std::istream& operator>>(std::istream& is, Book& b) {
    is >> static_cast<LibraryItem&>(b);
    std::cout << "  ISBN  : "; is >> b.isbn;
    std::cout << "  Pages : "; is >> b.pageCount;
    return is;
}
