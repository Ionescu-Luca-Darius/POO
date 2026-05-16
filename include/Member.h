#pragma once
#include <string>
#include <iostream>

class Member {
private:
    int memberId;
    std::string name;
    std::string email;
    std::string phone;
    int activeLoans;
    double fineOwed;
    static int nextId;

public:
    Member();
    Member(const std::string& name, const std::string& email, const std::string& phone);
    Member(const Member& other);
    Member& operator=(const Member& other);
    ~Member();

    int getMemberId() const;
    const std::string& getName() const;
    const std::string& getEmail() const;
    const std::string& getPhone() const;
    int getActiveLoans() const;
    double getFineOwed() const;

    void incrementLoans();
    void decrementLoans();
    void addFine(double amount);
    void payFine(double amount);

    bool operator==(const Member& other) const;
    bool operator<(const Member& other)  const;

    friend std::ostream& operator<<(std::ostream& os, const Member& m);
    friend std::istream& operator>>(std::istream& is, Member& m);
};
