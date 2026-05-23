#include "Library.h"
#include "Book.h"
#include "Magazine.h"
#include "DigitalItem.h"
#include "LibraryException.h"
#include <algorithm>
#include <iostream>

Library* Library::instance = nullptr;
int Library::totalCheckouts = 0;

void ActivityLogger::onCheckOut(const std::string& title) {
    log.push_back("CHECKOUT: " + title);
}
void ActivityLogger::onReturn(const std::string& title) {
    log.push_back("RETURN  : " + title);
}
void ActivityLogger::printLog() const {
    std::cout << "\n--- Activity Log ---\n";
    for (const auto& e : log) std::cout << "  " << e << "\n";
    if (log.empty()) std::cout << "  (no activity yet)\n";
}

StatisticsTracker::StatisticsTracker() : totalReturns(0) {}

void StatisticsTracker::onCheckOut(const std::string&) {
}
void StatisticsTracker::onReturn(const std::string&) {
    ++totalReturns;
}
void StatisticsTracker::recordType(const std::string& typeName) {
    checkoutsByType[typeName]++;
}
void StatisticsTracker::printStats() const {
    std::cout << "\n--- Checkout Statistics ---\n";
    if (checkoutsByType.empty()) { std::cout << "  (no checkouts yet)\n"; return; }
    for (const auto& [type, count] : checkoutsByType)
        std::cout << "  " << type << ": " << count << " checkout(s)\n";
    std::cout << "  Total returns: " << totalReturns << "\n";
}

Library* Library::getInstance() {
    if (!instance) instance = new Library();
    return instance;
}
Library* Library::getInstance(const std::string& name) {
    if (!instance) { instance = new Library(); instance->name = name; }
    return instance;
}
int Library::getTotalCheckouts() { return totalCheckouts; }

Library::~Library() {
    for (auto* item : items) delete item;
    items.clear();
    instance = nullptr;
}

void Library::addItem(LibraryItem* item) {
    if (!item) throw InvalidItemException("Cannot add null item.");
    items.push_back(item);
}

void Library::removeItem(const std::string& title) {
    auto it = std::find_if(items.begin(), items.end(),
        [&title](const LibraryItem* i) { return i->getTitle() == title; });
    if (it == items.end()) throw ItemNotFoundException(title);
    delete *it;
    items.erase(it);
}

void Library::checkOutItem(const std::string& title) {
    LibraryItem* item = findItem(title);
    item->checkOut();
    ++totalCheckouts;
    notifyCheckOut(title);
    notifyType(item->getType());
}

void Library::returnItem(const std::string& title) {
    LibraryItem* item = findItem(title);
    item->returnItem();
    notifyReturn(title);
}

LibraryItem* Library::findItem(const std::string& title) const {
    auto it = std::find_if(items.begin(), items.end(),
        [&title](const LibraryItem* i) { return i->getTitle() == title; });
    if (it == items.end()) throw ItemNotFoundException(title);
    return *it;
}

void Library::displayBooks() const {
    std::cout << "\n--- Books ---\n";
    bool found = false;
    for (const auto* item : items) {
        const Book* b = dynamic_cast<const Book*>(item);
        if (b) { b->displayDetails(); found = true; }
    }
    if (!found) std::cout << "  No books.\n";
}

void Library::displayDigitalItems() const {
    std::cout << "\n--- Digital Items ---\n";
    bool found = false;
    for (const auto* item : items) {
        const DigitalItem* d = dynamic_cast<const DigitalItem*>(item);
        if (d) { d->displayDetails(); found = true; }
    }
    if (!found) std::cout << "  No digital items.\n";
}

void Library::displayAll() const {
    std::cout << "\n=== All items in \"" << name << "\" ===\n";
    if (items.empty()) { std::cout << "  (empty)\n"; return; }
    for (const auto* item : items) item->displayDetails();
}

void Library::displayCheckedOut() const {
    std::cout << "\n--- Checked Out ---\n";
    bool found = false;
    for (const auto* item : items)
        if (item->isCheckedOut()) { std::cout << "  " << *item << "\n"; found = true; }
    if (!found) std::cout << "  None.\n";
}

void Library::sortByTitle() {
    std::sort(items.begin(), items.end(),
        [](const LibraryItem* a, const LibraryItem* b) { return *a < *b; });
}

void Library::addObserver(LibraryObserver* obs)    { observers.push_back(obs); }
void Library::removeObserver(LibraryObserver* obs) {
    observers.erase(std::remove(observers.begin(), observers.end(), obs),
                    observers.end());
}

void Library::notifyCheckOut(const std::string& title) {
    for (auto* obs : observers) obs->onCheckOut(title);
}
void Library::notifyReturn(const std::string& title) {
    for (auto* obs : observers) obs->onReturn(title);
}
void Library::notifyType(const std::string& typeName) {
    for (auto* obs : observers) {
        StatisticsTracker* st = dynamic_cast<StatisticsTracker*>(obs);
        if (st) st->recordType(typeName);
    }
}

const std::string& Library::getName() const { return name;}
int Library::size() const { return static_cast<int>(items.size());}
