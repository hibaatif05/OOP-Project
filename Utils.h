#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;
string getTodayDate();
// Convert DD-MM-YYYY to integer YYYYMMDD for sorting
int dateToInt(const string& date);
// Difference in days between two DD-MM-YYYY dates (a minus b)
long daysDiff(const string& dateA, const string& dateB);
// Compare two HH:MM time slot strings, returns <0 =0 >0
int compareTimeSlots(const string& a, const string& b);
// Case-insensitive string comparison
bool strEqualCI(const string& a, const string& b);
// Parse a comma-separated line into a vector of tokens.
// The last token captures everything so notes/medicines with commas are safe
vector<string> parseCSV(const string& line, int maxFields);
#endif
