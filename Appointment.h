#ifndef APPOINTMENT_H
#define APPOINTMENT_H
#include <iostream>
#include <string>
using namespace std;
class Appointment {
private:
    int appointmentId;
    int patientId;
    int doctorId;
    string date;
    string timeSlot;
    string status;
public:
    Appointment();
    Appointment(int apptId, int patId, int docId,
        const string& date, const string& timeSlot,
        const string& status);
    int getAppointmentId() const;
    int getPatientId() const;
    int getDoctorId() const;
    const string& getDate() const;
    const string& getTimeSlot() const;
    const string& getStatus() const;
    int getId() const;
    void setStatus(const string& s);
    bool operator==(const Appointment& o) const;
    friend ostream& operator<<(ostream& os, const Appointment& a);
};
#endif
