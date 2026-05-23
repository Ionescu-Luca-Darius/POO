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
    explicit ItemNotFoundException(const std::string& title)
        : LibraryException("Item not found: " + title) {}
};

class ItemAlreadyCheckedOutException : public LibraryException {
public:
    explicit ItemAlreadyCheckedOutException(const std::string& title)
        : LibraryException("Item already checked out: " + title) {}
};

class InvalidItemException : public LibraryException {
public:
    explicit InvalidItemException(const std::string& reason)
        : LibraryException("Invalid item data: " + reason) {}
};

class LoanLimitExceededException : public LibraryException {
public:
    explicit LoanLimitExceededException(const std::string& memberName, int limit)
        : LibraryException(memberName + " has reached the loan limit of "
                           + std::to_string(limit) + " items.") {}
};

class MemberNotFoundException : public LibraryException {
public:
    explicit MemberNotFoundException(int id)
        : LibraryException("Member not found: #" + std::to_string(id)) {}
};

class UnpaidFinesException : public LibraryException {
public:
    explicit UnpaidFinesException(const std::string& name, double amount)
        : LibraryException(name + " has unpaid fines of " +
                           std::to_string(amount) + " RON. Please pay before borrowing.") {}
};
