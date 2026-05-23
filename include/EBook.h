#pragma once
#include "DigitalItem.h"

class EBook : public DigitalItem {
private:
    int pageCount;
public:
    EBook();
    EBook(const std::string& title, const std::string& author,
          int year, const std::string& fileFormat,
          double fileSizeMB, int pageCount);
    EBook(const EBook& other);
    EBook& operator=(const EBook& other);
    ~EBook() override;

    std::string getType() const override;
    void displayDetails() const override;
    int getPageCount() const;

    friend std::ostream& operator<<(std::ostream& os, const EBook& e);
    friend std::istream& operator>>(std::istream& is, EBook& e);
};
