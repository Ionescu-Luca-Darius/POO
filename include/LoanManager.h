#pragma once
#include "Loan.h"
#include "Member.h"
#include "LibraryItem.h"
#include <list>
#include <map>
#include <vector>
#include <string>

class LoanManager {
private:
    std::map<int, Member*> members;
    std::list<Loan> loans;

    Member* findMember(int id) const;

public:
    LoanManager();
    ~LoanManager();

    void registerMember(Member* m);
    Member* getMember(int memberId) const;
    void displayAllMembers() const;

    int createLoan(int memberId, LibraryItem* item, const std::string& borrowDate);

    void closeLoan(int loanId, const std::string& returnDate, int daysLate);

    void markOverdueLoans(const std::string& today);

    void printLoanItemInfo(int loanId, const std::vector<LibraryItem*>& allItems) const;

    void displayActiveLoans() const;
    void displayOverdueLoans() const;
    void displayAllLoans() const;
    void displayMemberLoans(int memberId) const;

    int totalLoans() const;
    int activeLoansCount() const;
    double totalFinesCollected() const;
    double totalFinesPending() const;

    const std::list<Loan>& getLoans() const { return loans;}
    const std::map<int, Member*>& getMembers() const { return members;}
};
