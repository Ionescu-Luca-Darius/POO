#include "LibraryItem.h"
#include "LibraryException.h"

int LibraryItem::totalItemsCreated = 0;

LibraryItem::LibraryItem() : title("Unknown"), author("Unknown"), year(0), checkedOut(false)
{
    ++totalItemsCreated;
}

LibraryItem::LibraryItem(const std::string& title, const std::string& author, int year) : title(title), author(author), year(year), checkedOut(false)
{
    if (title.empty())
        throw InvalidItemException("Title cannot be empty.");
    if (year < 0 || year > 2100)
        throw InvalidItemException("Year out of range: " + std::to_string(year));
    ++totalItemsCreated;
}

LibraryItem::LibraryItem(const LibraryItem& other) : title(other.title), author(other.author), year(other.year), checkedOut(other.checkedOut)
{
    ++totalItemsCreated;
}

LibraryItem& LibraryItem::operator=(const LibraryItem& other) {
    if (this != &other) {
        title = other.title;
        author = other.author;
        year = other.year;
        checkedOut = other.checkedOut;
    }
    return *this;
}

LibraryItem::~LibraryItem() = default;

void LibraryItem::checkOut() {
    if (checkedOut)
        throw ItemAlreadyCheckedOutException(title);
    checkedOut = true;
}

void LibraryItem::returnItem() {
    checkedOut = false;
}

const std::string& LibraryItem::getTitle() const { return title; }

const std::string& LibraryItem::getAuthor() const { return author; }

int  LibraryItem::getYear() const { return year; }

bool LibraryItem::isCheckedOut() const { return checkedOut; }

int LibraryItem::getTotalItemsCreated() { return totalItemsCreated; }

bool LibraryItem::operator==(const LibraryItem& other) const {
    return title == other.title && author == other.author;
}

bool LibraryItem::operator<(const LibraryItem& other) const {
    return title < other.title;
}

std::ostream& operator<<(std::ostream& os, const LibraryItem& item) {
    os << "[" << item.getType() << "] \"" << item.title
       << "\" by " << item.author << " (" << item.year << ")"
       << (item.checkedOut ? " [CHECKED OUT]" : " [AVAILABLE]");
    return os;
}

std::istream& operator>>(std::istream& is, LibraryItem& item) {
    std::cout << "  Title : "; std::getline(is >> std::ws, item.title);
    std::cout << "  Author: "; std::getline(is >> std::ws, item.author);
    std::cout << "  Year  : "; is >> item.year;
    return is;
}
