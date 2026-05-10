#include "Utils.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <string>
using namespace std;
string getTodayDate() {
    time_t t = time(nullptr);
    tm tm_info{};
    localtime_s(&tm_info, &t);
    char buf[12];
    strftime(buf, sizeof(buf), "%d-%m-%Y", &tm_info);
    return string(buf);
}
int dateToInt(const string& date) {
    if (date.size() < 10) {
        return 0;
    }
    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));
    return (year * 10000) + (month * 100) + day;
}
long daysDiff(const string& dateA, const string& dateB) {
    auto parse = [](const string& d) -> time_t {
        struct tm tm = {};
        tm.tm_mday = stoi(d.substr(0, 2));
        tm.tm_mon = stoi(d.substr(3, 2)) - 1;
        tm.tm_year = stoi(d.substr(6, 4)) - 1900;
        tm.tm_isdst = -1;
        return mktime(&tm);
        };
    return static_cast<long>(difftime(parse(dateA), parse(dateB)) / 86400.0);
}
int compareTimeSlots(const string& a, const string& b) {
    return a.compare(b);
}
bool strEqualCI(const string& a, const string& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        char c1 = a[i];
        char c2 = b[i];
        if (c1 >= 'A' && c1 <= 'Z') {
            c1 += 32;
        }
        if (c2 >= 'A' && c2 <= 'Z') {
            c2 += 32;
        }
        if (c1 != c2) {
            return false;
        }
    }
    return true;
}
vector<string> parseCSV(const string& line, int maxFields) {
    vector<string> fields;
    string cur;
    for (char c : line) {
        if (c == ',' && (int)fields.size() < maxFields - 1) {
            fields.push_back(cur);
            cur.clear();
        }
        else if (c != '\r' && c != '\n') {
            cur += c;
        }
    }
    if (!cur.empty() || !fields.empty()) {
        fields.push_back(cur);
    }
    return fields;
}