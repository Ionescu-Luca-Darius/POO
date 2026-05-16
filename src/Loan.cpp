#include "Loan.h"

int Loan::nextLoanId = 1;
const double Loan::FINE_PER_DAY = 0.50;

std::string loanStatusToString(LoanStatus s) {
    switch (s) {
        case LoanStatus::ACTIVE: return "ACTIVE";
        case LoanStatus::RETURNED: return "RETURNED";
        case LoanStatus::OVERDUE: return "OVERDUE";
        default: return "UNKNOWN";
    }
}

Loan::Loan() : loanId(nextLoanId++), memberId(0), memberName(""), itemTitle(""), borrowDate(""), dueDate(""), returnDate(""), status(LoanStatus::ACTIVE), fineAmount(0.0) {}

Loan::Loan(int memberId, const std::string& memberName, const std::string& itemTitle, const std::string& borrowDate, const std::string& dueDate) : loanId(nextLoanId++), memberId(memberId), memberName(memberName), itemTitle(itemTitle), borrowDate(borrowDate), dueDate(dueDate), returnDate(""), status(LoanStatus::ACTIVE), fineAmount(0.0) {}

Loan::Loan(const Loan& other) : loanId(other.loanId), memberId(other.memberId), memberName(other.memberName), itemTitle(other.itemTitle), borrowDate(other.borrowDate), dueDate(other.dueDate), returnDate(other.returnDate), status(other.status), fineAmount(other.fineAmount) {}

Loan& Loan::operator=(const Loan& other) {
    if (this != &other) {
        memberId = other.memberId;
        memberName = other.memberName;
        itemTitle = other.itemTitle;
        borrowDate = other.borrowDate;
        dueDate = other.dueDate;
        returnDate = other.returnDate;
        status = other.status;
        fineAmount = other.fineAmount;
    }
    return *this;
}

Loan::~Loan() = default;

int Loan::getLoanId() const { return loanId; }
int Loan::getMemberId() const { return memberId; }
const std::string& Loan::getMemberName() const { return memberName; }
const std::string& Loan::getItemTitle() const { return itemTitle; }
const std::string& Loan::getBorrowDate() const { return borrowDate; }
const std::string& Loan::getDueDate() const { return dueDate; }
const std::string& Loan::getReturnDate() const { return returnDate; }
LoanStatus Loan::getStatus() const { return status; }
double Loan::getFineAmount() const { return fineAmount; }

void Loan::markReturned(const std::string& retDate, int daysLate) {
    returnDate = retDate;
    status = LoanStatus::RETURNED;
    if (daysLate > 0)
        fineAmount = daysLate * FINE_PER_DAY;
}

void Loan::markOverdue() {
    if (status == LoanStatus::ACTIVE)
        status = LoanStatus::OVERDUE;
}

bool Loan::isActive() const {
    return status == LoanStatus::ACTIVE || status == LoanStatus::OVERDUE;
}

bool Loan::operator==(const Loan& other) const {
    return loanId == other.loanId;
}

std::ostream& operator<<(std::ostream& os, const Loan& l) {
    os << "[Loan #" << l.loanId << "]"
       << "  Member: " << l.memberName << " (#" << l.memberId << ")"
       << "  Item: \"" << l.itemTitle << "\""
       << "  Borrowed: " << l.borrowDate
       << "  Due: " << l.dueDate
       << "  Status: " << loanStatusToString(l.status);
    if (!l.returnDate.empty())
        os << "  Returned: " << l.returnDate;
    if (l.fineAmount > 0.0)
        os << "  Fine: " << l.fineAmount << " RON";
    return os;
}
