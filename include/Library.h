#pragma once
#include "LibraryItem.h"
#include "LibraryException.h"
#include <vector>
#include <string>
#include <memory>

class LibraryObserver {
public:
    virtual void onCheckOut(const std::string& itemTitle) = 0;
    virtual void onReturn(const std::string& itemTitle) = 0;
    virtual ~LibraryObserver() = default;
};

class ActivityLogger : public LibraryObserver {
private:
    std::vector<std::string> log;
public:
    void onCheckOut(const std::string& title) override;
    void onReturn(const std::string& title) override;
    void printLog() const;
};

class Library {
private:
    static Library* instance;              

    std::vector<LibraryItem*> items;
    std::vector<LibraryObserver*> observers;
    std::string name;

    static int totalCheckouts;            

    Library() = default;
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    void notifyCheckOut(const std::string& title);
    void notifyReturn (const std::string& title);

public:
    static Library* getInstance();
    static Library* getInstance(const std::string& name);
    static int getTotalCheckouts();

    ~Library();

    void addItem(LibraryItem* item);
    void removeItem(const std::string& title);

    
    void checkOutItem(const std::string& title);
    void returnItem(const std::string& title);

    
    LibraryItem* findItem(const std::string& title) const;

    
    void displayBooks() const;
    void displayDigitalItems() const;
    void displayAll() const;
    void displayCheckedOut() const;

    void sortByTitle();

    void addObserver(LibraryObserver* obs);
    void removeObserver(LibraryObserver* obs);

    const std::string& getName() const;
    int size() const;
};
