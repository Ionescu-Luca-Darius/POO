#pragma once
#include "LibraryItem.h"

class DigitalItem : public LibraryItem {
protected:
    std::string fileFormat;   
    double fileSizeMB;

public:
    DigitalItem();
    DigitalItem(const std::string& title, const std::string& author, int year, const std::string& fileFormat, double fileSizeMB);
    DigitalItem(const DigitalItem& other);
    DigitalItem& operator=(const DigitalItem& other);
    ~DigitalItem() override;

    void checkOut() override;
    void returnItem() override;

    const std::string& getFileFormat() const;
    double getFileSizeMB() const;

    std::string getType() const override = 0;
    void displayDetails() const override = 0;

    friend std::ostream& operator<<(std::ostream& os, const DigitalItem& d);
    friend std::istream& operator>>(std::istream& is, DigitalItem& d);
};
