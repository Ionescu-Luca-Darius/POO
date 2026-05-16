#pragma once
#include <stdexcept>
#include <string>

class LibraryException : public std::exception {
protected:
    std::string message;
public:
    explicit LibraryException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
    virtual ~LibraryException() = default;
};

class ItemNotFoundException : public LibraryException {
public:
    explicit ItemNotFoundException(const std::string& title) : LibraryException("Item not found: " + title) {}
};

class ItemAlreadyCheckedOutException : public LibraryException {
public:
    explicit ItemAlreadyCheckedOutException(const std::string& title) : LibraryException("Item already checked out: " + title) {}
};

class InvalidItemException : public LibraryException {
public:
    explicit InvalidItemException(const std::string& reason) : LibraryException("Invalid item data: " + reason) {}
};
