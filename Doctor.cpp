#include "Doctor.h"
#include <iostream>
#include <iomanip>
using namespace std;
Doctor::Doctor() : Person(), fee(0.0f) {}
Doctor::Doctor(int id, const string& n, const string& spec,
    const string& c, const string& pwd, float f)
    : Person(id, n, pwd), specialization(spec), contact(c), fee(f) {
}
const string& Doctor::getSpecialization() const { 
    return specialization;
}
const string& Doctor::getContact() const { 
    return contact;
}
float Doctor::getFee() const {
    return fee;
}
void Doctor::setSpecialization(const string& s) {
    specialization = s;
}
void Doctor::setContact(const string& c) {
    contact = c;
}
void Doctor::setFee(float f) {
    fee = f;
}
bool Doctor::operator==(const Doctor& o) const {
    return id == o.id;
}
void Doctor::displayInfo() const {
    cout << "Doctor[" << id << "] " << name
        << " | " << specialization
        << " | " << contact
        << " | Fee: PKR " << fixed << setprecision(2) << fee << "\n";
}
ostream& operator<<(ostream& os, const Doctor& d) {
    os << "ID: " << d.id
        << " | Name: " << d.name
        << " | Spec: " << d.specialization
        << " | Contact: " << d.contact
        << " | Fee: PKR " << fixed << setprecision(2) << d.fee;
    return os;
}