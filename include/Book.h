#pragma once
#include "LibraryItem.h"

class Book : public LibraryItem {
private:
    std::string isbn;
    int pageCount;

    static PhysicalLoanPolicy policy;

public:
    Book();
    Book(const std::string& title, const std::string& author,
         int year, const std::string& isbn, int pageCount);
    Book(const Book& other);
    Book& operator=(const Book& other);
    ~Book() override;

    std::string getType() const override;
    void displayDetails() const override;
    const LoanPolicy& getLoanPolicy() const override;

    const std::string& getIsbn() const;
    int getPageCount() const;

    bool operator>(const Book& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Book& b);
    friend std::istream& operator>>(std::istream& is, Book& b);
};
