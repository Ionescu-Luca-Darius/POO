#pragma once
#include <string>
#include <iostream>

class LoanPolicy {
public:
    virtual ~LoanPolicy() = default;

    virtual int  getLoanDurationDays() const = 0;

    virtual double getFinePerDay() const = 0;

    virtual bool allowsConcurrentLoans()  const = 0;

    virtual std::string getPolicyName()   const = 0;

    std::string computeDueDate(const std::string& borrowDate) const;

    friend std::ostream& operator<<(std::ostream& os, const LoanPolicy& p);
};

class PhysicalLoanPolicy : public LoanPolicy {
public:
    int getLoanDurationDays() const override { return 14;    }
    double getFinePerDay() const override { return 0.50;  }
    bool allowsConcurrentLoans() const override { return false; }
    std::string getPolicyName() const override { return "Physical (14 days, 0.50 RON/day)"; }
};

class DigitalLoanPolicy : public LoanPolicy {
public:
    int getLoanDurationDays() const override { return 21;    }
    double getFinePerDay() const override { return 0.25;  }
    bool allowsConcurrentLoans() const override { return true;  }
    std::string getPolicyName() const override { return "Digital (21 days, 0.25 RON/day)"; }
};

class ReferenceLoanPolicy : public LoanPolicy {
public:
    int getLoanDurationDays() const override { return 3;     }
    double getFinePerDay() const override { return 1.00;  }
    bool allowsConcurrentLoans() const override { return false; }
    std::string getPolicyName() const override { return "Reference (3 days, 1.00 RON/day)"; }
};
