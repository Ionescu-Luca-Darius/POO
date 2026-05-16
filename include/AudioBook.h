#pragma once
#include "DigitalItem.h"

class AudioBook : public DigitalItem {
private:
    double durationHours;
    std::string narrator;

public:
    AudioBook();
    AudioBook(const std::string& title, const std::string& author, int year, const std::string& fileFormat, double fileSizeMB, double durationHours, const std::string& narrator);
    AudioBook(const AudioBook& other);
    AudioBook& operator=(const AudioBook& other);
    ~AudioBook() override;

    std::string getType() const override;
    void displayDetails() const override;
    double getDurationHours() const;
    const std::string& getNarrator() const;

    friend std::ostream& operator<<(std::ostream& os, const AudioBook& a);
    friend std::istream& operator>>(std::istream& is, AudioBook& a);
};
