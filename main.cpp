#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include "Library.h"
#include "Book.h"
#include "Magazine.h"
#include "EBook.h"
#include "AudioBook.h"
#include "Catalog.h"
#include "LibraryException.h"
#include "Member.h"
#include "Loan.h"
#include "LoanManager.h"
#include "Report.h"

static void printMainMenu() {
    std::cout << "\n";
    std::cout << "LIBRARY MANAGEMENT SYSTEM \n";
    std::cout << "\n  --- Collection ---\n";
    std::cout << "  1.  Display all items\n";
    std::cout << "  2.  Display books only\n";
    std::cout << "  3.  Display digital items only\n";
    std::cout << "  4.  Sort items by title\n";
    std::cout << "  5.  Show book catalog\n";
    std::cout << "  6.  Show magazine catalog\n";
    std::cout << "\n  --- Lending ---\n";
    std::cout << "  7.  Check out an item\n";
    std::cout << "  8.  Return an item\n";
    std::cout << "  9.  Display currently checked-out items\n";
    std::cout << "\n  --- Members ---\n";
    std::cout << " 10.  Display all members\n";
    std::cout << " 11.  Look up member loans\n";
    std::cout << " 12.  Pay member fine\n";
    std::cout << "\n  --- Loans ---\n";
    std::cout << " 13.  Create a loan (with member tracking)\n";
    std::cout << " 14.  Close a loan (item returned)\n";
    std::cout << " 15.  Display active loans\n";
    std::cout << " 16.  Display overdue loans\n";
    std::cout << " 17.  Display all loans\n";
    std::cout << " 18.  Mark overdue (enter today's date)\n";
    std::cout << "\n  --- Reports ---\n";
    std::cout << " 19.  Full report\n";
    std::cout << " 20.  Fines report\n";
    std::cout << " 21.  Daily summary\n";
    std::cout << " 22.  Activity log\n";
    std::cout << " 23.  Statistics\n";
    std::cout << "\n   0.  Exit\n";
    std::cout << "\nChoice: ";
}

static std::string readLine(const std::string& prompt) {
    std::string line;
    std::cout << prompt;
    std::getline(std::cin >> std::ws, line);
    return line;
}

static int readInt(const std::string& prompt) {
    int val;
    std::cout << prompt;
    std::cin >> val;
    return val;
}

static void loadSampleData(Library* lib, LoanManager& lm) {
    lib->addItem(new Book("The C++ Programming Language",
                          "Bjarne Stroustrup", 2013,
                          "978-0321563842", 1376));
    lib->addItem(new Book("Clean Code",
                          "Robert C. Martin", 2008,
                          "978-0132350884", 431));
    lib->addItem(new Book("Design Patterns",
                          "Gang of Four", 1994,
                          "978-0201633610", 395));
    lib->addItem(new Book("A Brief History of Time",
                          "Stephen Hawking", 1988,
                          "978-0553380163", 212));
    lib->addItem(new Book("Zero to One",
                          "Peter Thiel", 2014,
                          "978-0804139021", 195));

    lib->addItem(new Magazine("Scientific American",
                              "Various Authors", 2024, 312, "Science"));
    lib->addItem(new Magazine("National Geographic",
                              "Various Authors", 2023, 489, "Nature"));
    lib->addItem(new Magazine("Wired",
                              "Various Authors", 2024, 32, "Tech"));

    lib->addItem(new EBook("Effective Modern C++",
                           "Scott Meyers", 2014, "EPUB", 4.2, 334));

    lib->addItem(new AudioBook("Atomic Habits",
                               "James Clear", 2018,
                               "MP3", 320.0, 5.5, "James Clear"));

    lm.registerMember(new Member("Alice Popescu",     "alice@example.com",  "0721000001"));
    lm.registerMember(new Member("Bogdan Ionescu",    "bogdan@example.com", "0721000002"));
    lm.registerMember(new Member("Carmen Dumitrescu", "carmen@example.com", "0721000003"));

    lib->checkOutItem("Clean Code");
    lm.createLoan(1000, "Clean Code", "2025-03-01", "2025-03-15");

    lib->checkOutItem("Atomic Habits");
    lm.createLoan(1001, "Atomic Habits", "2025-04-01", "2025-04-21");

    std::cout << "  Sample data loaded.\n";
}

int main() {
    Library* lib = Library::getInstance("City Central Library");

    ActivityLogger logger;
    lib->addObserver(&logger);

    LoanManager loanManager;

    Catalog<Book> bookCatalog("Physical Books");
    Catalog<Magazine> magCatalog ("Magazines");

    std::cout << "\nLoading sample data...\n";
    loadSampleData(lib, loanManager);

    bookCatalog.add(dynamic_cast<Book*>(lib->findItem("The C++ Programming Language")));
    bookCatalog.add(dynamic_cast<Book*>(lib->findItem("Clean Code")));
    bookCatalog.add(dynamic_cast<Book*>(lib->findItem("Design Patterns")));
    bookCatalog.add(dynamic_cast<Book*>(lib->findItem("A Brief History of Time")));
    bookCatalog.add(dynamic_cast<Book*>(lib->findItem("Zero to One")));

    magCatalog.add(dynamic_cast<Magazine*>(lib->findItem("Scientific American")));
    magCatalog.add(dynamic_cast<Magazine*>(lib->findItem("National Geographic")));
    magCatalog.add(dynamic_cast<Magazine*>(lib->findItem("Wired")));

    int choice = -1;
    while (choice != 0) {
        printMainMenu();
        std::cin >> choice;

        switch (choice) {

        case 1:
            lib->displayAll();
            break;

        case 2:
            lib->displayBooks();
            break;

        case 3:
            lib->displayDigitalItems();
            break;

        case 4:
            lib->sortByTitle();
            std::cout << "  Items sorted by title.\n";
            break;

        case 5:
            bookCatalog.sortByTitle();
            bookCatalog.display();
            break;

        case 6:
            magCatalog.sortByTitle();
            magCatalog.display();
            break;

        case 7: {
            std::string title = readLine("  Title to check out: ");
            try {
                lib->checkOutItem(title);
                std::cout << "  Checked out: \"" << title << "\"\n";
            } catch (const ItemAlreadyCheckedOutException& e) {
                std::cout << "  Error: " << e.what() << "\n";
            } catch (const ItemNotFoundException& e) {
                std::cout << "  Error: " << e.what() << "\n";
            } catch (const LibraryException& e) {
                std::cout << "  Library error: " << e.what() << "\n";
            }
            break;
        }

        case 8: {
            std::string title = readLine("  Title to return: ");
            try {
                lib->returnItem(title);
                std::cout << "  Returned: \"" << title << "\"\n";
            } catch (const LibraryException& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            break;
        }

        case 9:
            lib->displayCheckedOut();
            break;

        case 10:
            loanManager.displayAllMembers();
            break;

        case 11: {
            int id = readInt("  Member ID: ");
            try {
                loanManager.displayMemberLoans(id);
            } catch (const std::exception& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            break;
        }

        case 12: {
            int id = readInt("  Member ID: ");
            try {
                Member* m = loanManager.getMember(id);
                std::cout << "  Current fine: " << m->getFineOwed() << " RON\n";
                double amt;
                std::cout << "  Amount to pay: ";
                std::cin >> amt;
                m->payFine(amt);
                std::cout << "  Remaining fine: " << m->getFineOwed() << " RON\n";
            } catch (const std::exception& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            break;
        }

        case 13: {
            std::string title = readLine("  Item title    : ");
            int memId = readInt( "  Member ID    : ");
            std::string borrow = readLine("  Borrow date (YYYY-MM-DD): ");
            std::string due = readLine("  Due date    (YYYY-MM-DD): ");
            try {
                lib->checkOutItem(title);
                int loanId = loanManager.createLoan(memId, title, borrow, due);
                std::cout << "  Loan #" << loanId << " created for \"" << title << "\".\n";
            } catch (const ItemAlreadyCheckedOutException& e) {
                std::cout << "  Error: " << e.what() << "\n";
            } catch (const std::exception& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            break;
        }

        case 14: {
            int loanId = readInt("  Loan ID      : ");
            std::string ret = readLine("  Return date (YYYY-MM-DD): ");
            int daysLate = readInt( "  Days late (0 = on time): ");
            try {
                const auto& allLoans = loanManager.getLoans();
                std::string itemTitle;
                for (const auto* l : allLoans)
                    if (l->getLoanId() == loanId) { itemTitle = l->getItemTitle(); break; }

                loanManager.closeLoan(loanId, ret, daysLate);

                if (!itemTitle.empty()) {
                    try { lib->returnItem(itemTitle); } catch (...) {}
                }
                std::cout << "  Loan #" << loanId << " closed.\n";
                if (daysLate > 0)
                    std::cout << "  Fine applied: " << (daysLate * 0.5) << " RON\n";
            } catch (const std::exception& e) {
                std::cout << "  Error: " << e.what() << "\n";
            }
            break;
        }

        case 15:
            loanManager.displayActiveLoans();
            break;

        case 16:
            loanManager.displayOverdueLoans();
            break;

        case 17:
            loanManager.displayAllLoans();
            break;

        case 18: {
            std::string today = readLine("  Today's date (YYYY-MM-DD): ");
            loanManager.markOverdueLoans(today);
            break;
        }

        case 19: {
            Report r(*lib, loanManager);
            r.printFullReport();
            break;
        }

        case 20: {
            Report r(*lib, loanManager);
            r.printFinesReport();
            break;
        }

        case 21: {
            std::string date = readLine("  Date for summary (YYYY-MM-DD): ");
            Report r(*lib, loanManager);
            r.printDailySummary(date);
            break;
        }

        case 22:
            logger.printLog();
            break;

        case 23:
            std::cout << "\n--- Statistics ---\n";
            std::cout << "  Library              : " << lib->getName() << "\n";
            std::cout << "  Total items          : " << lib->size() << "\n";
            std::cout << "  Total items created  : " << LibraryItem::getTotalItemsCreated() << "\n";
            std::cout << "  Session checkouts    : " << Library::getTotalCheckouts() << "\n";
            std::cout << "  Registered members   : " << loanManager.getMembers().size() << "\n";
            std::cout << "  Total loans          : " << loanManager.totalLoans() << "\n";
            std::cout << "  Active loans         : " << loanManager.activeLoans() << "\n";
            std::cout << "  Fines pending (RON)  : " << loanManager.totalFinesPending() << "\n";
            break;

        case 0:
            std::cout << "  Goodbye!\n";
            break;

        default:
            std::cout << "  Invalid option. Please try again.\n";
        }
    }

    delete lib;
    return 0;
}