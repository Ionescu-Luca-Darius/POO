#pragma once
#include <string>
#include <iostream>

class Member {
protected:
    int memberId;
    std::string name;
    std::string email;
    std::string phone;
    int activeLoans;
    double fineOwed;

    static int nextId;

public:
    Member();
    Member(const std::string& name, const std::string& email,
           const std::string& phone);
    Member(const Member& other);
    Member& operator=(const Member& other);
    virtual ~Member();

    virtual std::string getMemberType()  const = 0;

    virtual int getMaxLoans() const = 0;
    virtual int getExtraGraceDays() const = 0;
    virtual bool canRequestPurchase() const = 0;

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

    bool canBorrow() const;

    bool operator==(const Member& other) const;
    bool operator<(const Member& other)  const;

    friend std::ostream& operator<<(std::ostream& os, const Member& m);
    friend std::istream& operator>>(std::istream& is, Member& m);
};

class StudentMember : public Member {
public:
    StudentMember();
    StudentMember(const std::string& name, const std::string& email,
                  const std::string& phone);
    StudentMember(const StudentMember& other);
    StudentMember& operator=(const StudentMember& other);
    ~StudentMember() override;

    std::string getMemberType() const override { return "Student";}
    int getMaxLoans() const override { return 3;}
    int getExtraGraceDays() const override { return 0;}
    bool canRequestPurchase() const override { return false;}
};

class FacultyMember : public Member {
private:
    std::string department;
public:
    FacultyMember();
    FacultyMember(const std::string& name, const std::string& email,
                  const std::string& phone, const std::string& department);
    FacultyMember(const FacultyMember& other);
    FacultyMember& operator=(const FacultyMember& other);
    ~FacultyMember() override;

    std::string getMemberType() const override { return "Faculty";}
    int getMaxLoans() const override { return 8;}
    int getExtraGraceDays() const override { return 3;}
    bool canRequestPurchase() const override { return true;}

    const std::string& getDepartment() const;
};

bool sameDepartment(const FacultyMember& a, const FacultyMember& b);
