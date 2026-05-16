#pragma once
#include <string>
#include <iostream>

enum class LoanStatus {
    ACTIVE,
    RETURNED,
    OVERDUE
};

std::string loanStatusToString(LoanStatus s);

class Loan {
private:
    int loanId;
    int memberId;
    std::string memberName;
    std::string itemTitle;
    std::string borrowDate;
    std::string dueDate;
    std::string returnDate;
    LoanStatus status;
    double fineAmount;

    static int nextLoanId;
    static const double FINE_PER_DAY;

public:
    Loan();
    Loan(int memberId, const std::string& memberName, const std::string& itemTitle, const std::string& borrowDate, const std::string& dueDate);
    Loan(const Loan& other);
    Loan& operator=(const Loan& other);
    ~Loan();

    int getLoanId() const;
    int getMemberId() const;
    const std::string& getMemberName() const;
    const std::string& getItemTitle() const;
    const std::string& getBorrowDate() const;
    const std::string& getDueDate() const;
    const std::string& getReturnDate() const;
    LoanStatus getStatus() const;
    double getFineAmount() const;

    void markReturned(const std::string& returnDate, int daysLate = 0);

    void markOverdue();

    bool isActive() const;

    bool operator==(const Loan& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Loan& l);
};
