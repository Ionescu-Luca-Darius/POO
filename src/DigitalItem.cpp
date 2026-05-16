#include "DigitalItem.h"
#include "LibraryException.h"
#include <iostream>

DigitalItem::DigitalItem() : LibraryItem(), fileFormat("EPUB"), fileSizeMB(0.0) {}

DigitalItem::DigitalItem(const std::string& title, const std::string& author, int year, const std::string& fileFormat, double fileSizeMB) : LibraryItem(title, author, year), fileFormat(fileFormat), fileSizeMB(fileSizeMB)
{
    if (fileSizeMB < 0)
        throw InvalidItemException("File size cannot be negative.");
}

DigitalItem::DigitalItem(const DigitalItem& other) : LibraryItem(other), fileFormat(other.fileFormat), fileSizeMB(other.fileSizeMB) {}

DigitalItem& DigitalItem::operator=(const DigitalItem& other) {
    if (this != &other) {
        LibraryItem::operator=(other);
        fileFormat  = other.fileFormat;
        fileSizeMB  = other.fileSizeMB;
    }
    return *this;
}

DigitalItem::~DigitalItem() = default;

void DigitalItem::checkOut() {
    checkedOut = true;
}

void DigitalItem::returnItem() {
    checkedOut = false;
}

const std::string& DigitalItem::getFileFormat() const { return fileFormat; }

double DigitalItem::getFileSizeMB() const { return fileSizeMB; }

std::ostream& operator<<(std::ostream& os, const DigitalItem& d) {
    os << static_cast<const LibraryItem&>(d)
       << "  Format: " << d.fileFormat
       << "  Size: "   << d.fileSizeMB << " MB";
    return os;
}

std::istream& operator>>(std::istream& is, DigitalItem& d) {
    is >> static_cast<LibraryItem&>(d);
    std::cout << "  Format: "; is >> d.fileFormat;
    std::cout << "  Size MB: "; is >> d.fileSizeMB;
    return is;
}
