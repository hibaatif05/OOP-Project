#ifndef ADMIN_H
#define ADMIN_H
#include <iostream>
#include "Person.h"
using namespace std;
class Admin : public Person {
public:
    Admin();
    Admin(int id, const string& name, const string& password);
    void displayInfo() const  override;
};
#endif
