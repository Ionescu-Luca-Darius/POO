#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <iostream>


template <typename T>
class Catalog {
private:
    std::vector<T*> items;
    std::string catalogName;

public:
    explicit Catalog(const std::string& name) : catalogName(name) {}

    Catalog(const Catalog&) = delete;
    Catalog& operator=(const Catalog&) = delete;

    ~Catalog() {
    }

    void add(T* item) {
        if (!item) throw std::invalid_argument("Cannot add null item to catalog.");
        items.push_back(item);
    }

    void remove(const std::string& title) {
        auto it = std::find_if(items.begin(), items.end(),
            [&title](const T* item) { return item->getTitle() == title; });
        if (it == items.end())
            throw std::runtime_error("Item '" + title + "' not found in catalog.");
        items.erase(it);
    }

    T* find(const std::string& title) const {
        auto it = std::find_if(items.begin(), items.end(),
            [&title](const T* item) { return item->getTitle() == title; });
        if (it == items.end()) return nullptr;
        return *it;
    }

    void sortByTitle() {
        std::sort(items.begin(), items.end(),
            [](const T* a, const T* b) { return *a < *b; });
    }

    void display() const {
        std::cout << "\n=== Catalog: " << catalogName << " ===\n";
        if (items.empty()) { std::cout << "  (empty)\n"; return; }
        for (const auto* item : items)
            std::cout << *item << "\n";
    }

    int  size() const { return static_cast<int>(items.size()); }
    const std::string& getName() const { return catalogName; }
};
