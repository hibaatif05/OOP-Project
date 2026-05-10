#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <iostream>
#include <string>
using namespace std;
class HospitalException {
protected:
    string message;
public:
    explicit HospitalException(const string& msg) : message(msg) {}
    virtual string what() const {
        return message;
    }
    virtual ~HospitalException() = default;
};
class FileNotFoundException : public HospitalException {
public:
    explicit FileNotFoundException(const string& filename)
        : HospitalException("File not found: " + filename) {
    }
};
class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException()
        : HospitalException("Insufficient balance. Please top up your account.") {
    }
};
class InvalidInputException : public HospitalException {
public:
    explicit InvalidInputException(const string& msg)
        : HospitalException(msg) {
    }
};
class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException()
        : HospitalException("The selected time slot is already booked. Please choose another slot.") {
    }
};
#endif
