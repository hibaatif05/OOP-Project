#include "Person.h"
Person::Person() : id(0) {}
Person::Person(int i, const string& n, const string& p)
    : id(i), name(n), password(p) {
}
int Person::getId() const {
    return id;
}
const string& Person::getName() const {
    return name;
}
const string& Person::getPassword() const {
    return password;
}
void Person::setId(int i) {
    id = i;
}
void Person::setName(const string& n) {
    name = n;
}
void Person::setPassword(const string& p) {
    password = p;
}