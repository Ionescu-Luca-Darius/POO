#pragma once
#include "LoanManager.h"
#include "Library.h"
#include <string>

class Report {
private:
    const Library& library;
    const LoanManager& loanManager;

public:
    Report(const Library& lib, const LoanManager& lm);

    void printFullReport() const;

    void printCheckedOutReport() const;

    void printFinesReport() const;

    void printOverdueReport() const;

    void printDailySummary(const std::string& date) const;
};
