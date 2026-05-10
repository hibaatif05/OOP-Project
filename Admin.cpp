#include "Admin.h"
#include <iostream>
using namespace std;
Admin::Admin() : Person() {}
Admin::Admin(int id, const string& n, const string& pwd)
    : Person(id, n, pwd) {
}
void Admin::displayInfo() const {
    cout << "Admin[" << id << "] " << name << "\n";
}