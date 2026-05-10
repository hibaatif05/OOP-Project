#ifndef BILL_H
#define BILL_H
#include <iostream>
#include <string>
using namespace std;
class Bill {
private:
    int billId;
    int patientId;
    int appointmentId;
    float amount;
    string status;
    string date;
public:
    Bill();
    Bill(int billId, int patientId, int appointmentId,
        float amount, const string& status, const string& date);
    int getBillId() const;
    int getPatientId() const;
    int getAppointmentId() const;
    float getAmount() const;
    const string& getStatus() const;
    const string& getDate() const;
    int getId() const;
    void setStatus(const string& s);
};
#endif
