#include "Patient.h"
#include <iostream>
#include <iomanip>
using namespace std;
Patient::Patient() : Person(), age(0), balance(0.0f) {}
Patient::Patient(int id, const string& n, int a,
    const string& g, const string& c,
    const string& pwd, float bal)
    : Person(id, n, pwd), age(a), gender(g), contact(c), balance(bal) {
}
int Patient::getAge() const {
    return age;
}
const string& Patient::getGender() const {
    return gender;
}
const string& Patient::getContact() const {
    return contact;
}
float Patient::getBalance() const {
    return balance;
}
void Patient::setAge(int a) {
    age = a;
}
void Patient::setGender(const string& g) {
    gender = g;
}
void Patient::setContact(const string& c) {
    contact = c;
}
void Patient::setBalance(float b) {
    balance = b;
}
Patient& Patient::operator+=(float amount) {
    balance += amount;
    return *this;
}
Patient& Patient::operator-=(float amount) {
    balance -= amount;
    return *this;
}
bool Patient::operator==(const Patient& o) const {
    return id == o.id;
}
void Patient::displayInfo() const {
    cout << "Patient[" << id << "] " << name
        << " | Age: " << age
        << " | " << gender
        << " | " << contact
        << " | Balance: PKR "
        << fixed << setprecision(2) << balance << "\n";
}
ostream& operator<<(ostream& os, const Patient& p) {
    os << "ID: " << p.id
        << " | Name: " << p.name
        << " | Age: " << p.age
        << " | " << p.gender
        << " | " << p.contact
        << " | Balance: PKR "
        << fixed << setprecision(2) << p.balance;
    return os;
}