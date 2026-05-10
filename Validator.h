#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <iostream>
#include <string>
using namespace std;
class Validator {
public:
    static bool isValidId(const string& s);
    static bool isValidDate(const string& s);      // DD-MM-YYYY, year >= current
    static bool isValidTimeSlot(const string& s);  // one of the 8 fixed slots
    static bool isValidContact(const string& s);   // exactly 11 digits
    static bool isValidPassword(const string& s);  // minimum 6 chars
    static bool isValidPositiveFloat(const string& s);
    static bool isValidMenuChoice(const string& s, int minVal, int maxVal);
    static int  getCurrentYear();
};
#endif
