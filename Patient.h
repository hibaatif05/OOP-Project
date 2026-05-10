#ifndef PATIENT_H
#define PATIENT_H
#include "Person.h"
#include <iostream>
using namespace std;
class Patient : public Person {
private:
    int age;
    string gender;
    string contact;
    float balance;
public:
    Patient();
    Patient(int id, const string& name, int age,
        const string& gender, const string& contact,
        const string& password, float balance);
    int getAge() const;
    const string& getGender() const;
    const string& getContact() const;
    float getBalance() const;
    void setAge(int a);
    void setGender(const string& g);
    void setContact(const string& c);
    void setBalance(float b);
    Patient& operator+=(float amount);
    Patient& operator-=(float amount);
    bool operator==(const Patient& o) const;
    void displayInfo() const override;
    friend ostream& operator<<(ostream& os, const Patient& p);
};
#endif