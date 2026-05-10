#include "Appointment.h"
#include <iostream>
using namespace std;
Appointment::Appointment() : appointmentId(0), patientId(0), doctorId(0) {}
Appointment::Appointment(int apptId, int patId, int docId,
    const string& d, const string& ts,
    const string& st)
    : appointmentId(apptId), patientId(patId), doctorId(docId),
    date(d), timeSlot(ts), status(st) {
}
int Appointment::getAppointmentId() const {
    return appointmentId; 
}
int Appointment::getPatientId() const { 
    return patientId;
}
int Appointment::getDoctorId() const {
    return doctorId; 
}
const string& Appointment::getDate() const { 
    return date; 
}
const string& Appointment::getTimeSlot() const {
    return timeSlot; 
}
const string& Appointment::getStatus() const {
    return status;
}
int Appointment::getId() const {
    return appointmentId;
}
void Appointment::setStatus(const string& s) {
    status = s;
}
bool Appointment::operator==(const Appointment& o) const {
    return doctorId == o.doctorId
        && date == o.date
        && timeSlot == o.timeSlot
        && status != "cancelled"
        && o.status != "cancelled";
}
ostream& operator<<(ostream& os, const Appointment& a) {
    os << "ApptID: " << a.appointmentId
        << " | Pat: " << a.patientId
        << " | Doc: " << a.doctorId
        << " | " << a.date
        << " | " << a.timeSlot
        << " | " << a.status;
    return os;
}