#include "Validator.h"
#include <ctime>
#include <string>
using namespace std;
static const string VALID_SLOTS[] = {
    "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00"
};
static const int SLOT_COUNT = 8;
int Validator::getCurrentYear() {
    time_t t = time(nullptr);
    tm tm_info{};
    localtime_s(&tm_info, &t);
    // tm_year is years since 1900
    return tm_info.tm_year + 1900;
}
bool Validator::isValidId(const string& s) {
    if (s.empty()) {
        return false;
    }
    for (char c : s) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    try {
        return stoi(s) > 0;
    }
    catch (...) {
        return false;
    }
}
bool Validator::isValidDate(const string& s) {
    if (s.size() != 10) {
        return false;
    }
    if (s[2] != '-' || s[5] != '-') {
        return false;
    }
    int positions[] = { 0, 1, 3, 4, 6, 7, 8, 9 };
    for (int i : positions) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    try {
        int day = stoi(s.substr(0, 2));
        int month = stoi(s.substr(3, 2));
        int year = stoi(s.substr(6, 4));
        if (day < 1 || day > 31) {
            return false;
        }
        if (month < 1 || month > 12) {
            return false;
        }
        if (year < getCurrentYear()) {
            return false;
        }
    }
    catch (...) {
        return false;
    }
    return true;
}
bool Validator::isValidTimeSlot(const string& s) {
    for (int i = 0; i < SLOT_COUNT; ++i) {
        if (s == VALID_SLOTS[i]) {
            return true;
        }
    }
    return false;
}
bool Validator::isValidContact(const string& s) {
    if (s.size() != 11) {
        return false;
    }
    for (char c : s) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}
bool Validator::isValidPassword(const string& s) {
    return s.size() >= 6;
}
bool Validator::isValidPositiveFloat(const string& s) {
    if (s.empty()) {
        return false;
    }
    bool hasDot = false;
    bool hasDigit = false;
    for (char c : s) {
        if (c == '.') {
            if (hasDot) {
                return false;
            }
            hasDot = true;
        }
        else if (c >= '0' && c <= '9') {
            hasDigit = true;
        }
        else {
            return false;
        }
    }
    if (!hasDigit) {
        return false;
    }
    try {
        return stof(s) > 0.0f;
    }
    catch (...) {
        return false;
    }
}
bool Validator::isValidMenuChoice(const string& s, int minVal, int maxVal) {
    if (s.empty()) {
        return false;
    }
    for (char c : s) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    try {
        int v = stoi(s);
        return v >= minVal && v <= maxVal;
    }
    catch (...) {
        return false;
    }
}