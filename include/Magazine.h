#pragma once
#include "LibraryItem.h"

class Magazine : public LibraryItem {
private:
    int issueNumber;
    std::string category;   

public:
    Magazine();
    Magazine(const std::string& title, const std::string& author, int year, int issueNumber, const std::string& category);
    Magazine(const Magazine& other);
    Magazine& operator=(const Magazine& other);
    ~Magazine() override;

    std::string getType() const override;
    void displayDetails() const override;

    int getIssueNumber() const;
    const std::string& getCategory() const;

    bool operator>(const Magazine& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Magazine& m);
    friend std::istream& operator>>(std::istream& is, Magazine& m);
};
