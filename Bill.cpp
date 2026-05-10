#include "Bill.h"
#include <iostream>
using namespace std;
Bill::Bill() : billId(0), patientId(0), appointmentId(0), amount(0.0f) {}
Bill::Bill(int bid, int pid, int aid, float amt,
    const string& st, const string& dt)
    : billId(bid), patientId(pid), appointmentId(aid),
    amount(amt), status(st), date(dt) {
}
int Bill::getBillId() const {
    return billId;
}
int Bill::getPatientId() const { 
    return patientId;
}
int Bill::getAppointmentId() const { 
    return appointmentId;
}
float Bill::getAmount() const { 
    return amount; 
}
const string& Bill::getStatus() const {
    return status; 
}
const string& Bill::getDate() const { 
    return date;
}
int Bill::getId() const { 
    return billId; 
}
void Bill::setStatus(const string& s) { 
    status = s;
}