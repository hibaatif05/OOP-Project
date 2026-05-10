#ifndef PERSON_H
#define PERSON_H
#include <string>
using namespace std;
class Person {
protected:
    int id;
    string name;
    string password;
public:
    Person();
    Person(int id, const string& name, const string& password);
    virtual ~Person() {}
    int getId() const;
    const string& getName() const;
    const string& getPassword() const;
    void setId(int id);
    void setName(const string& n);
    void setPassword(const string& p);
    virtual void displayInfo() const = 0;
};
#endif