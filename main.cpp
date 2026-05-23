#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Library.h"
#include "Book.h"
#include "Magazine.h"
#include "EBook.h"
#include "AudioBook.h"
#include "Catalog.h"
#include "LibraryException.h"
#include "Member.h"
#include "LoanManager.h"
#include "Report.h"

static void printMenu() {
    std::cout << "\n";
    std::cout << "LIBRARY MANAGEMENT SYSTEM\n";
    std::cout << "\n";
    std::cout << "  --- Collection ---\n";
    std::cout << "  1.  Display all items\n";
    std::cout << "  2.  Display books only\n";
    std::cout << "  3.  Display digital items only\n";
    std::cout << "  4.  Sort by title\n";
    std::cout << "  5.  Book catalog\n";
    std::cout << "  6.  Magazine catalog\n";
    std::cout << "  --- Lending ---\n";
    std::cout << "  7.  Borrow item (policy-driven)\n";
    std::cout << "  8.  Return item\n";
    std::cout << "  9.  Checked-out items\n";
    std::cout << " 10.  Loan item details (downcast demo)\n";
    std::cout << "  --- Members ---\n";
    std::cout << " 11.  All members\n";
    std::cout << " 12.  Member loan history\n";
    std::cout << " 13.  Pay fine\n";
    std::cout << " 14.  Check member eligibility\n";
    std::cout << "  --- Loans ---\n";
    std::cout << " 15.  Active loans\n";
    std::cout << " 16.  Overdue loans\n";
    std::cout << " 17.  All loans\n";
    std::cout << " 18.  Close loan\n";
    std::cout << " 19.  Mark overdue (enter date)\n";
    std::cout << "  --- Reports ---\n";
    std::cout << " 20.  Full report\n";
    std::cout << " 21.  Fines report\n";
    std::cout << " 22.  Overdue report\n";
    std::cout << " 23.  Run all reports\n";
    std::cout << "  --- Stats ---\n";
    std::cout << " 24.  Activity log\n";
    std::cout << " 25.  Checkout statistics\n";
    std::cout << " 26.  General statistics\n";
    std::cout << "   0.  Exit\n";
    std::cout << "Choice: ";
}

static std::string readLine(const std::string& prompt) {
    std::string line;
    std::cout << prompt;
    std::getline(std::cin >> std::ws, line);
    return line;
}
static int readInt(const std::string& prompt) {
    int v; std::cout << prompt; std::cin >> v; return v;
}

static void loadSampleData(Library* lib, LoanManager& lm) {
    lib->addItem(new Book("The C++ Programming Language",
                          "Bjarne Stroustrup", 2013, "978-0321563842", 1376));
    lib->addItem(new Book("Clean Code",
                          "Robert C. Martin", 2008, "978-0132350884", 431));
    lib->addItem(new Book("Design Patterns",
                          "Gang of Four", 1994, "978-0201633610", 395));
    lib->addItem(new Book("A Brief History of Time",
                          "Stephen Hawking", 1988, "978-0553380163", 212));
    lib->addItem(new Book("Zero to One",
                          "Peter Thiel", 2014, "978-0804139021", 195));
    lib->addItem(new Magazine("Scientific American",
                              "Various", 2024, 312, "Science"));
    lib->addItem(new Magazine("National Geographic",
                              "Various", 2023, 489, "Nature"));
    lib->addItem(new Magazine("Wired",
                              "Various", 2024, 32, "Tech"));
    lib->addItem(new EBook("Effective Modern C++",
                           "Scott Meyers", 2014, "EPUB", 4.2, 334));
    lib->addItem(new AudioBook("Atomic Habits",
                               "James Clear", 2018, "MP3", 320.0, 5.5, "James Clear"));

    lm.registerMember(new StudentMember("Alice Popescu",     "alice@uni.ro",  "0721000001"));
    lm.registerMember(new StudentMember("Bogdan Ionescu",    "bogdan@uni.ro", "0721000002"));
    lm.registerMember(new FacultyMember("Carmen Dumitrescu", "carmen@uni.ro", "0721000003", "Computer Science"));
    lm.registerMember(new FacultyMember("Dan Marinescu",     "dan@uni.ro",    "0721000004", "Mathematics"));

    try {
        LibraryItem* cleanCode = lib->findItem("Clean Code");
        lib->checkOutItem("Clean Code");
        lm.createLoan(1000, cleanCode, "2025-03-01");

        LibraryItem* atomicHabits = lib->findItem("Atomic Habits");
        lib->checkOutItem("Atomic Habits");
        lm.createLoan(1001, atomicHabits, "2025-04-10");

        LibraryItem* wired = lib->findItem("Wired");
        lib->checkOutItem("Wired");
        lm.createLoan(1002, wired, "2025-04-20");
    } catch (const LibraryException& e) {
        std::cout << "  Setup warning: " << e.what() << "\n";
    }

    std::cout << "  Sample data loaded.\n";
}

int main() {
    Library* lib = Library::getInstance("City Central Library");

    ActivityLogger logger;
    StatisticsTracker tracker;
    lib->addObserver(&logger);
    lib->addObserver(&tracker);

    LoanManager loanManager;

    Catalog<Book> bookCatalog("Physical Books");
    Catalog<Magazine> magCatalog ("Magazines");

    std::cout << "\nLoading sample data...\n";
    loadSampleData(lib, loanManager);

    for (auto* item : lib->getItems()) {
        if (Book* b = dynamic_cast<Book*>(item))
            bookCatalog.add(b);
        else if (Magazine* m = dynamic_cast<Magazine*>(item))
            magCatalog.add(m);
    }

    std::vector<Report*> reports = {
        new FullReport(*lib, loanManager),
        new FinesReport(*lib, loanManager),
        new OverdueReport(*lib, loanManager)
    };

    int choice = -1;
    while (choice != 0) {
        printMenu();
        std::cin >> choice;

        switch (choice) {

        case 1: lib->displayAll(); break;
        case 2: lib->displayBooks(); break;
        case 3: lib->displayDigitalItems(); break;

        case 4:
            lib->sortByTitle();
            std::cout << "  Sorted by title.\n";
            break;

        case 5: bookCatalog.sortByTitle(); bookCatalog.display(); break;
        case 6: magCatalog.sortByTitle();  magCatalog.display();  break;

        case 7: {
            std::string title = readLine("  Item title    : ");
            int memId = readInt( "  Member ID     : ");
            std::string date = readLine("  Borrow date (YYYY-MM-DD): ");
            try {
                LibraryItem* item = lib->findItem(title);

                lib->checkOutItem(title);

                int loanId = loanManager.createLoan(memId, item, date);
                std::cout << "  Loan #" << loanId << " created.\n";

            } catch (const LoanLimitExceededException& e) {
                std::cout << "  Loan limit: " << e.what() << "\n";
            } catch (const UnpaidFinesException& e) {
                std::cout << "  Fines outstanding: " << e.what() << "\n";
            } catch (const ItemAlreadyCheckedOutException& e) {
                std::cout << "  Already out: " << e.what() << "\n";
            } catch (const LibraryException& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            break;
        }

        case 8: {
            std::string title = readLine("  Item title: ");
            try {
                lib->returnItem(title);
                std::cout << "  Returned: \"" << title << "\"\n";
            } catch (const LibraryException& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            break;
        }

        case 9:  lib->displayCheckedOut(); break;

        case 10: {
            int loanId = readInt("  Loan ID: ");
            loanManager.printLoanItemInfo(loanId, lib->getItems());
            break;
        }

        case 11: loanManager.displayAllMembers(); break;

        case 12: {
            int id = readInt("  Member ID: ");
            try { loanManager.displayMemberLoans(id); }
            catch (const LibraryException& e) { std::cout << "  " << e.what() << "\n"; }
            break;
        }

        case 13: {
            int id = readInt("  Member ID: ");
            try {
                Member* m = loanManager.getMember(id);
                std::cout << "  Current fine: " << m->getFineOwed() << " RON\n";
                double amt; std::cout << "  Pay amount: "; std::cin >> amt;
                m->payFine(amt);
                std::cout << "  Remaining: " << m->getFineOwed() << " RON\n";
            } catch (const LibraryException& e) { std::cout << "  " << e.what() << "\n"; }
            break;
        }

        case 14: {
            int id = readInt("  Member ID: ");
            try {
                Member* m = loanManager.getMember(id);
                std::cout << "\n  Member: " << m->getName()
                          << " [" << m->getMemberType() << "]\n";
                std::cout << "  Max loans      : " << m->getMaxLoans() << "\n";
                std::cout << "  Active loans   : " << m->getActiveLoans() << "\n";
                std::cout << "  Grace days     : " << m->getExtraGraceDays() << "\n";
                std::cout << "  Can request    : " << (m->canRequestPurchase() ? "yes" : "no") << "\n";
                std::cout << "  Can borrow now : " << (m->canBorrow() ? "yes" : "no") << "\n";

                FacultyMember* fm = dynamic_cast<FacultyMember*>(m);
                if (fm)
                    std::cout << "  Department     : " << fm->getDepartment() << "\n";
            } catch (const LibraryException& e) { std::cout << "  " << e.what() << "\n"; }
            break;
        }

        case 15: loanManager.displayActiveLoans();  break;
        case 16: loanManager.displayOverdueLoans(); break;
        case 17: loanManager.displayAllLoans(); break;

        case 18: {
            int loanId = readInt( "  Loan ID  : ");
            std::string retDate = readLine("  Return date (YYYY-MM-DD): ");
            int daysLate = readInt( "  Days late (0 = on time) : ");
            try {
                loanManager.closeLoan(loanId, retDate, daysLate);
                const auto& loans = loanManager.getLoans();
                for (const auto& l : loans) {
                    if (l.getLoanId() == loanId) {
                        try { lib->returnItem(l.getItemTitle()); } catch (...) {}
                        break;
                    }
                }
                std::cout << "  Loan #" << loanId << " closed.\n";
            } catch (const LibraryException& e) { std::cout << "  " << e.what() << "\n"; }
            break;
        }

        case 19: {
            std::string today = readLine("  Today's date (YYYY-MM-DD): ");
            loanManager.markOverdueLoans(today);
            break;
        }

        case 20: reports[0]->generate(); break;
        case 21: reports[1]->generate(); break;
        case 22: reports[2]->generate(); break;

        case 23:
            std::cout << "\n=== Running all reports ===\n";
            for (Report* r : reports) {
                std::cout << "\n>> " << r->getReportName() << "\n";
                r->generate();
            }
            break;

        case 24: logger.printLog();   break;
        case 25: tracker.printStats(); break;

        case 26:
            std::cout << "\n--- General Statistics ---\n";
            std::cout << "  Library            : " << lib->getName() << "\n";
            std::cout << "  Items              : " << lib->size() << "\n";
            std::cout << "  Total ever created : " << LibraryItem::getTotalItemsCreated() << "\n";
            std::cout << "  Session checkouts  : " << Library::getTotalCheckouts() << "\n";
            std::cout << "  Members            : " << loanManager.getMembers().size() << "\n";
            std::cout << "  Total loans        : " << loanManager.totalLoans() << "\n";
            std::cout << "  Active loans       : " << loanManager.activeLoansCount() << "\n";
            std::cout << "  Fines pending (RON): " << loanManager.totalFinesPending() << "\n";
            break;

        case 0:
            std::cout << "  Goodbye!\n";
            break;

        default:
            std::cout << "  Invalid option.\n";
        }
    }

    for (Report* r : reports) delete r;
    delete lib;
    return 0;
}
