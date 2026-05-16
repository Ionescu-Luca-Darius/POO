#pragma once
#include <iostream>
#include <string>

class LibraryItem {
protected:
    std::string title;
    std::string author;
    int year;
    bool checkedOut;
    static int totalItemsCreated;   

public:
   
    LibraryItem();
    LibraryItem(const std::string& title, const std::string& author, int year);
    LibraryItem(const LibraryItem& other);
    LibraryItem& operator=(const LibraryItem& other);
    virtual ~LibraryItem();

    virtual std::string getType() const = 0;
    virtual void displayDetails() const = 0;

    virtual void checkOut();
    virtual void returnItem();

    const std::string& getTitle() const;
    const std::string& getAuthor() const;
    int getYear() const;
    bool isCheckedOut() const;

    
    static int getTotalItemsCreated();

    
    bool operator==(const LibraryItem& other) const;
    bool operator<(const LibraryItem& other) const;

    friend std::ostream& operator<<(std::ostream& os, const LibraryItem& item);
    friend std::istream& operator>>(std::istream& is, LibraryItem& item);
};
