#ifndef DOCTOR_H
#define DOCTOR_H
#include "Person.h"
#include <iostream>
using namespace std;
class Doctor : public Person {
private:
    string specialization;
    string contact;
    float fee;
public:
    Doctor();
    Doctor(int id, const string& name, const string& specialization,
        const string& contact, const string& password, float fee);
    const string& getSpecialization() const;
    const string& getContact() const;
    float getFee() const;
    void setSpecialization(const string& s);
    void setContact(const string& c);
    void setFee(float f);
    bool operator==(const Doctor& o) const;
    void displayInfo() const override;
    friend ostream& operator<<(ostream& os, const Doctor& d);
};
#endif
