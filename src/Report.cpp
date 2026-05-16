#include "Report.h"
#include <iostream>
#include <vector>
#include <algorithm>

Report::Report(const Library& lib, const LoanManager& lm) : library(lib), loanManager(lm) {}

static void printSeparator(char c = '-', int w = 50) {
    for (int i = 0; i < w; ++i) std::cout << c;
    std::cout << "\n";
}

void Report::printFullReport() const {
    printSeparator('=');
    std::cout << "  FULL LIBRARY REPORT\n";
    printSeparator('=');
    std::cout << "\n[COLLECTION]\n";
    std::cout << "  Total items in library : " << library.size() << "\n";
    std::cout << "  Total items ever created: "
              << LibraryItem::getTotalItemsCreated() << "\n";
    std::cout << "  Total checkouts (session): "
              << Library::getTotalCheckouts() << "\n";

    std::cout << "\n[MEMBERS]\n";
    const auto& members = loanManager.getMembers();
    std::cout << "  Registered members: " << members.size() << "\n";
    int membersWithFines = 0;
    for (const auto* m : members)
        if (m->getFineOwed() > 0) ++membersWithFines;
    std::cout << "  Members with unpaid fines: " << membersWithFines << "\n";

    std::cout << "\n[LOANS]\n";
    std::cout << "  Total loans ever: " << loanManager.totalLoans() << "\n";
    std::cout << "  Currently active: " << loanManager.activeLoans() << "\n";
    std::cout << "  Fines collected : " << loanManager.totalFinesCollected() << " RON\n";
    std::cout << "  Fines pending   : " << loanManager.totalFinesPending()   << " RON\n";

    printSeparator('=');
}

void Report::printCheckedOutReport() const {
    printSeparator();
    std::cout << "  CURRENTLY CHECKED-OUT ITEMS\n";
    printSeparator();

    const auto& loans = loanManager.getLoans();
    bool any = false;
    for (const auto* l : loans) {
        if (l->isActive()) {
            std::cout << "  \"" << l->getItemTitle() << "\""
                      << "  ->  " << l->getMemberName()
                      << "  (due: " << l->getDueDate() << ")\n";
            any = true;
        }
    }
    if (!any) std::cout << "  No items currently checked out.\n";
    printSeparator();
}

void Report::printFinesReport() const {
    printSeparator();
    std::cout << "  UNPAID FINES REPORT\n";
    printSeparator();

    const auto& members = loanManager.getMembers();
    std::vector<const Member*> withFines;
    for (const auto* m : members)
        if (m->getFineOwed() > 0.0)
            withFines.push_back(m);

    if (withFines.empty()) {
        std::cout << "  No unpaid fines. Everyone is square!\n";
        printSeparator();
        return;
    }

    std::sort(withFines.begin(), withFines.end(),
        [](const Member* a, const Member* b) {
            return a->getFineOwed() > b->getFineOwed();
        });

    for (const auto* m : withFines)
        std::cout << "  " << m->getName()
                  << "  ->  " << m->getFineOwed() << " RON\n";
    printSeparator();
}

void Report::printOverdueReport() const {
    printSeparator();
    std::cout << "  OVERDUE LOANS REPORT\n";
    printSeparator();

    const auto& loans = loanManager.getLoans();
    bool any = false;
    for (const auto* l : loans) {
        if (l->getStatus() == LoanStatus::OVERDUE) {
            std::cout << "  [OVERDUE] \"" << l->getItemTitle() << "\""
                      << "  Member: " << l->getMemberName()
                      << "  Due: " << l->getDueDate() << "\n";
            any = true;
        }
    }
    if (!any) std::cout << "  No overdue loans.\n";
    printSeparator();
}

void Report::printDailySummary(const std::string& date) const {
    std::cout << "\n  Daily Summary [" << date << "]"
              << "  Library: " << library.getName() << "\n";
    std::cout << "  Items: " << library.size()
              << "  |  Active loans: " << loanManager.activeLoans()
              << "  |  Members: " << loanManager.getMembers().size()
              << "  |  Checkouts today (session): " << Library::getTotalCheckouts()
              << "\n";
}
