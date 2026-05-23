#include "Report.h"
#include <iostream>
#include <vector>
#include <algorithm>

Report::Report(const Library& lib, const LoanManager& lm)
    : library(lib), loanManager(lm) {}

void Report::printSeparator(char c, int w) {
    for (int i = 0; i < w; ++i) std::cout << c;
    std::cout << "\n";
}

FullReport::FullReport(const Library& lib, const LoanManager& lm)
    : Report(lib, lm) {}

void FullReport::generate() const {
    printSeparator('=');
    std::cout << "  " << getReportName() << "\n";
    printSeparator('=');

    std::cout << "\n[COLLECTION]\n";
    std::cout << "  Items in library       : " << library.size() << "\n";
    std::cout << "  Total ever created     : " << LibraryItem::getTotalItemsCreated() << "\n";
    std::cout << "  Session checkouts      : " << Library::getTotalCheckouts() << "\n";

    std::cout << "\n[MEMBERS]\n";
    int students = 0, faculty = 0, withFines = 0;
    for (const auto& [id, m] : loanManager.getMembers()) {
        if (m->getMemberType() == "Student") ++students;
        else ++faculty;
        if (m->getFineOwed() > 0) ++withFines;
    }
    std::cout << "  Students               : " << students << "\n";
    std::cout << "  Faculty                : " << faculty  << "\n";
    std::cout << "  Members with fines     : " << withFines << "\n";

    std::cout << "\n[LOANS]\n";
    std::cout << "  Total loans            : " << loanManager.totalLoans() << "\n";
    std::cout << "  Active                 : " << loanManager.activeLoansCount() << "\n";
    std::cout << "  Fines collected (RON)  : " << loanManager.totalFinesCollected() << "\n";
    std::cout << "  Fines pending   (RON)  : " << loanManager.totalFinesPending() << "\n";
    printSeparator('=');
}

FinesReport::FinesReport(const Library& lib, const LoanManager& lm)
    : Report(lib, lm) {}

void FinesReport::generate() const {
    printSeparator();
    std::cout << "  " << getReportName() << "\n";
    printSeparator();

    std::vector<const Member*> withFines;
    for (const auto& [id, m] : loanManager.getMembers())
        if (m->getFineOwed() > 0.0)
            withFines.push_back(m);

    if (withFines.empty()) {
        std::cout << "  No unpaid fines.\n";
        printSeparator();
        return;
    }

    std::sort(withFines.begin(), withFines.end(),
        [](const Member* a, const Member* b) {
            return a->getFineOwed() > b->getFineOwed();
        });

    for (const auto* m : withFines)
        std::cout << "  [" << m->getMemberType() << "] " << m->getName()
                  << "  ->  " << m->getFineOwed() << " RON"
                  << "  (grace days: " << m->getExtraGraceDays() << ")\n";

    printSeparator();
}

OverdueReport::OverdueReport(const Library& lib, const LoanManager& lm)
    : Report(lib, lm) {}

void OverdueReport::generate() const {
    printSeparator();
    std::cout << "  " << getReportName() << "\n";
    printSeparator();

    bool any = false;
    for (const auto& l : loanManager.getLoans()) {
        if (l.getStatus() == LoanStatus::OVERDUE) {
            std::cout << "  \"" << l.getItemTitle() << "\""
                      << "  Borrower: " << l.getMemberName()
                      << "  Due: " << l.getDueDate()
                      << "  Rate: " << l.getFinePerDay() << " RON/day\n";
            any = true;
        }
    }
    if (!any) std::cout << "  No overdue loans.\n";
    printSeparator();
}
