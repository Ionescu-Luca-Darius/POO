#include "LoanManager.h"
#include "LibraryException.h"
#include "DigitalItem.h"
#include "Book.h"
#include "Magazine.h"
#include "EBook.h"
#include "AudioBook.h"
#include <iostream>
#include <algorithm>
#include <vector>

LoanManager::LoanManager() {}

LoanManager::~LoanManager() {
    for (auto& [id, m] : members)
        delete m;
}

Member* LoanManager::findMember(int id) const {
    auto it = members.find(id);
    if (it == members.end())
        throw MemberNotFoundException(id);
    return it->second;
}

void LoanManager::registerMember(Member* m) {
    if (!m) throw InvalidItemException("Cannot register null member.");
    members[m->getMemberId()] = m;
}

Member* LoanManager::getMember(int memberId) const {
    return findMember(memberId);
}

void LoanManager::displayAllMembers() const {
    std::cout << "\n--- Registered Members ---\n";
    if (members.empty()) { std::cout << "  (none)\n"; return; }
    for (const auto& [id, m] : members)
        std::cout << "  " << *m << "\n";
}

int LoanManager::createLoan(int memberId, LibraryItem* item,
                             const std::string& borrowDate) {
    Member* m = findMember(memberId);

    if (m->getActiveLoans() >= m->getMaxLoans())
        throw LoanLimitExceededException(m->getName(), m->getMaxLoans());

    double fineThreshold = (m->getExtraGraceDays() > 0) ? 10.0 : 0.0;
    if (m->getFineOwed() > fineThreshold)
        throw UnpaidFinesException(m->getName(), m->getFineOwed());

    const LoanPolicy& policy = item->getLoanPolicy();

    if (!policy.allowsConcurrentLoans() && item->isCheckedOut())
        throw ItemAlreadyCheckedOutException(item->getTitle());

    std::string dueDate = policy.computeDueDate(borrowDate);

    loans.emplace_back(memberId, m->getName(), item->getTitle(),
                       borrowDate, dueDate);
    m->incrementLoans();

    std::cout << "  Loan policy applied: " << policy.getPolicyName() << "\n";
    std::cout << "  Due date computed  : " << dueDate << "\n";

    return loans.back().getLoanId();
}

void LoanManager::closeLoan(int loanId, const std::string& returnDate, int daysLate) {
    for (auto& l : loans) {
        if (l.getLoanId() != loanId) continue;

        if (!l.isActive())
            throw LibraryException("Loan #" + std::to_string(loanId) + " is already closed.");

        Member* m = findMember(l.getMemberId());

        double fine = daysLate * l.getFinePerDay();
        l.markReturned(returnDate, daysLate);

        m->decrementLoans();
        if (fine > 0.0) {
            m->addFine(fine);
            std::cout << "  Fine applied: " << fine << " RON ("
                      << daysLate << " days x " << l.getFinePerDay() << " RON/day)\n";
        }
        return;
    }
    throw ItemNotFoundException("Loan #" + std::to_string(loanId));
}

void LoanManager::markOverdueLoans(const std::string& today) {
    int count = 0;
    for (auto& l : loans) {
        if (l.isActive() && l.getDueDate() < today) {
            l.markOverdue();
            ++count;
        }
    }
    std::cout << "  Marked " << count << " loan(s) as overdue.\n";
}

void LoanManager::printLoanItemInfo(int loanId,
                                    const std::vector<LibraryItem*>& allItems) const {
    const Loan* found = nullptr;
    for (const auto& l : loans)
        if (l.getLoanId() == loanId) { found = &l; break; }

    if (!found) {
        std::cout << "  Loan #" << loanId << " not found.\n";
        return;
    }

    LibraryItem* item = nullptr;
    for (auto* i : allItems)
        if (i->getTitle() == found->getItemTitle()) { item = i; break; }

    if (!item) {
        std::cout << "  Item no longer in library.\n";
        return;
    }

    std::cout << "\n  Loan #" << loanId << " — item details:\n";
    std::cout << "  Type: " << item->getType() << "\n";
    std::cout << "  Policy: " << item->getLoanPolicy().getPolicyName() << "\n";

    if (const Book* b = dynamic_cast<const Book*>(item)) {
        std::cout << "  ISBN: " << b->getIsbn()
                  << "  Pages: " << b->getPageCount() << "\n";
    } else if (const AudioBook* ab = dynamic_cast<const AudioBook*>(item)) {
        std::cout << "  Narrator: " << ab->getNarrator()
                  << "  Duration: " << ab->getDurationHours() << "h\n";
    } else if (const EBook* eb = dynamic_cast<const EBook*>(item)) {
        std::cout << "  Pages: " << eb->getPageCount()
                  << "  Format: " << eb->getFileFormat() << "\n";
    } else if (const Magazine* mg = dynamic_cast<const Magazine*>(item)) {
        std::cout << "  Issue #" << mg->getIssueNumber()
                  << "  Category: " << mg->getCategory() << "\n";
    }
}

void LoanManager::displayActiveLoans() const {
    std::cout << "\n--- Active Loans ---\n";
    bool any = false;
    for (const auto& l : loans)
        if (l.getStatus() == LoanStatus::ACTIVE) {
            std::cout << "  " << l << "\n"; any = true;
        }
    if (!any) std::cout << "  (none)\n";
}

void LoanManager::displayOverdueLoans() const {
    std::cout << "\n--- Overdue Loans ---\n";
    bool any = false;
    for (const auto& l : loans)
        if (l.getStatus() == LoanStatus::OVERDUE) {
            std::cout << "  " << l << "\n"; any = true;
        }
    if (!any) std::cout << "  (none)\n";
}

void LoanManager::displayAllLoans() const {
    std::cout << "\n--- All Loans (" << loans.size() << " total) ---\n";
    if (loans.empty()) { std::cout << "  (none)\n"; return; }
    for (const auto& l : loans)
        std::cout << "  " << l << "\n";
}

void LoanManager::displayMemberLoans(int memberId) const {
    Member* m = findMember(memberId);
    std::cout << "\n--- Loans for " << m->getName() << " ---\n";
    bool any = false;
    for (const auto& l : loans)
        if (l.getMemberId() == memberId) { std::cout << "  " << l << "\n"; any = true; }
    if (!any) std::cout << "  No loans on record.\n";
}

int LoanManager::totalLoans() const { return static_cast<int>(loans.size()); }

int LoanManager::activeLoansCount() const {
    return static_cast<int>(
        std::count_if(loans.begin(), loans.end(),
            [](const Loan& l) { return l.isActive(); }));
}

double LoanManager::totalFinesCollected() const {
    double total = 0.0;
    for (const auto& l : loans)
        if (l.getStatus() == LoanStatus::RETURNED)
            total += l.getFineAmount();
    return total;
}

double LoanManager::totalFinesPending() const {
    double total = 0.0;
    for (const auto& [id, m] : members)
        total += m->getFineOwed();
    return total;
}
