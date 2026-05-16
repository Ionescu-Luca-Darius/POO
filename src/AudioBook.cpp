#include "AudioBook.h"
#include "LibraryException.h"
#include <iostream>

AudioBook::AudioBook() : DigitalItem(), durationHours(0.0), narrator("Unknown") {}

AudioBook::AudioBook(const std::string& title, const std::string& author, int year, const std::string& fileFormat, double fileSizeMB, double durationHours, const std::string& narrator) : DigitalItem(title, author, year, fileFormat, fileSizeMB), durationHours(durationHours), narrator(narrator)
{
    if (durationHours < 0)
        throw InvalidItemException("Duration cannot be negative.");
}

AudioBook::AudioBook(const AudioBook& other) : DigitalItem(other), durationHours(other.durationHours), narrator(other.narrator) {}

AudioBook& AudioBook::operator=(const AudioBook& other) {
    if (this != &other) {
        DigitalItem::operator=(other);
        durationHours = other.durationHours;
        narrator      = other.narrator;
    }
    return *this;
}

AudioBook::~AudioBook() = default;

std::string AudioBook::getType() const { return "AudioBook"; }

void AudioBook::displayDetails() const {
    std::cout << *this << "\n"
              << "   Duration: " << durationHours << "h"
              << "  Narrator: " << narrator << "\n";
}

double AudioBook::getDurationHours() const { return durationHours; }

const std::string& AudioBook::getNarrator() const { return narrator; }

std::ostream& operator<<(std::ostream& os, const AudioBook& a) {
    os << static_cast<const DigitalItem&>(a)
       << "  Duration: " << a.durationHours << "h"
       << "  Narrator: " << a.narrator;
    return os;
}

std::istream& operator>>(std::istream& is, AudioBook& a) {
    is >> static_cast<DigitalItem&>(a);
    std::cout << "  Duration (hours): "; is >> a.durationHours;
    std::cout << "  Narrator: "; is >> std::ws; std::getline(is, a.narrator);
    return is;
}
