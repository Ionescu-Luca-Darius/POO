#include "LoanManager.h"
#include "LibraryException.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

LoanManager::LoanManager() {}

LoanManager::~LoanManager() {
    for (auto* l : loans)   delete l;
    for (auto* m : members) delete m;
}

Member* LoanManager::findMemberById(int id) const {
    for (auto* m : members)
        if (m->getMemberId() == id) return m;
    return nullptr;
}

Member* LoanManager::findMemberByName(const std::string& name) const {
    for (auto* m : members)
        if (m->getName() == name) return m;
    return nullptr;
}

void LoanManager::registerMember(Member* m) {
    if (!m) throw std::invalid_argument("Cannot register null member.");
    members.push_back(m);
}

Member* LoanManager::getMember(int memberId) const {
    Member* m = findMemberById(memberId);
    if (!m) throw std::runtime_error("Member not found: " + std::to_string(memberId));
    return m;
}

Member* LoanManager::getMemberByName(const std::string& name) const {
    return findMemberByName(name);
}

void LoanManager::displayAllMembers() const {
    std::cout << "\n--- Registered Members ---\n";
    if (members.empty()) { std::cout << "  (none)\n"; return; }
    for (const auto* m : members)
        std::cout << "  " << *m << "\n";
}

int LoanManager::createLoan(int memberId, const std::string& itemTitle, const std::string& borrowDate, const std::string& dueDate) {
    Member* m = findMemberById(memberId);
    if (!m) throw std::runtime_error("Member not found: " + std::to_string(memberId));

    if (m->getActiveLoans() >= 5)
        throw std::runtime_error(m->getName() + " has reached the maximum loan limit (5).");

    auto* loan = new Loan(memberId, m->getName(), itemTitle, borrowDate, dueDate);
    loans.push_back(loan);
    m->incrementLoans();
    return loan->getLoanId();
}

void LoanManager::closeLoan(int loanId, const std::string& returnDate, int daysLate) {
    for (auto* l : loans) {
        if (l->getLoanId() == loanId) {
            if (!l->isActive())
                throw std::runtime_error("Loan #" + std::to_string(loanId) + " is already closed.");
            l->markReturned(returnDate, daysLate);

            Member* m = findMemberById(l->getMemberId());
            if (m) {
                m->decrementLoans();
                if (l->getFineAmount() > 0)
                    m->addFine(l->getFineAmount());
            }
            return;
        }
    }
    throw std::runtime_error("Loan not found: " + std::to_string(loanId));
}

void LoanManager::markOverdueLoans(const std::string& today) {
    int count = 0;
    for (auto* l : loans) {
        if (l->isActive() && l->getDueDate() < today) {
            l->markOverdue();
            ++count;
        }
    }
    std::cout << "  Marked " << count << " loan(s) as overdue.\n";
}

void LoanManager::displayActiveLoans() const {
    std::cout << "\n--- Active Loans ---\n";
    bool any = false;
    for (const auto* l : loans)
        if (l->getStatus() == LoanStatus::ACTIVE) {
            std::cout << "  " << *l << "\n";
            any = true;
        }
    if (!any) std::cout << "  (none)\n";
}

void LoanManager::displayReturnedLoans() const {
    std::cout << "\n--- Returned Loans ---\n";
    bool any = false;
    for (const auto* l : loans)
        if (l->getStatus() == LoanStatus::RETURNED) {
            std::cout << "  " << *l << "\n";
            any = true;
        }
    if (!any) std::cout << "  (none)\n";
}

void LoanManager::displayOverdueLoans() const {
    std::cout << "\n--- Overdue Loans ---\n";
    bool any = false;
    for (const auto* l : loans)
        if (l->getStatus() == LoanStatus::OVERDUE) {
            std::cout << "  " << *l << "\n";
            any = true;
        }
    if (!any) std::cout << "  (none — great!)\n";
}

void LoanManager::displayAllLoans() const {
    std::cout << "\n--- All Loans (" << loans.size() << " total) ---\n";
    if (loans.empty()) { std::cout << "  (none)\n"; return; }
    for (const auto* l : loans)
        std::cout << "  " << *l << "\n";
}

void LoanManager::displayMemberLoans(int memberId) const {
    Member* m = findMemberById(memberId);
    if (!m) { std::cout << "  Member not found.\n"; return; }

    std::cout << "\n--- Loans for " << m->getName() << " ---\n";
    bool any = false;
    for (const auto* l : loans) {
        if (l->getMemberId() == memberId) {
            std::cout << "  " << *l << "\n";
            any = true;
        }
    }
    if (!any) std::cout << "  No loans found.\n";
}

int LoanManager::totalLoans() const {
    return static_cast<int>(loans.size());
}

int LoanManager::activeLoans() const {
    int cnt = 0;
    for (const auto* l : loans)
        if (l->isActive()) ++cnt;
    return cnt;
}

double LoanManager::totalFinesCollected() const {
    double total = 0.0;
    for (const auto* l : loans)
        if (l->getStatus() == LoanStatus::RETURNED)
            total += l->getFineAmount();
    return total;
}

double LoanManager::totalFinesPending() const {
    double total = 0.0;
    for (const auto* m : members)
        total += m->getFineOwed();
    return total;
}
