#pragma once
#include "LoanManager.h"
#include "Library.h"
#include <string>

class Report {
protected:
    const Library& library;
    const LoanManager& loanManager;

    static void printSeparator(char c = '-', int w = 50);

public:
    Report(const Library& lib, const LoanManager& lm);
    virtual ~Report() = default;

    virtual void generate() const = 0;

    virtual std::string getReportName() const = 0;
};

class FullReport : public Report {
public:
    FullReport(const Library& lib, const LoanManager& lm);
    void generate() const override;
    std::string getReportName()  const override { return "Full Library Report"; }
};

class FinesReport : public Report {
public:
    FinesReport(const Library& lib, const LoanManager& lm);
    void generate() const override;
    std::string getReportName() const override { return "Unpaid Fines Report"; }
};

class OverdueReport : public Report {
public:
    OverdueReport(const Library& lib, const LoanManager& lm);
    void generate() const override;
    std::string getReportName()  const override { return "Overdue Loans Report"; }
};
