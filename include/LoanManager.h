#pragma once
#include "Loan.h"
#include "Member.h"
#include <vector>
#include <string>

class LoanManager {
private:
    std::vector<Loan*> loans;
    std::vector<Member*> members;

    Member* findMemberById(int id) const;
    Member* findMemberByName(const std::string&) const;

public:
    LoanManager();
    ~LoanManager();

    void registerMember(Member* m);
    Member* getMember(int memberId) const;
    Member* getMemberByName(const std::string& name) const;
    void displayAllMembers() const;

    int  createLoan(int memberId, const std::string& itemTitle, const std::string& borrowDate, const std::string& dueDate);

    void closeLoan(int loanId, const std::string& returnDate, int daysLate = 0);

    void markOverdueLoans(const std::string& today);

    void displayActiveLoans() const;
    void displayReturnedLoans() const;
    void displayOverdueLoans() const;
    void displayAllLoans() const;
    void displayMemberLoans(int memberId) const;

    int totalLoans() const;
    int activeLoans() const;
    double totalFinesCollected() const;
    double totalFinesPending() const;

    const std::vector<Loan*>& getLoans() const { return loans; }
    const std::vector<Member*>& getMembers() const { return members; }
};
