#include "Prescription.h"
#include <iostream>
using namespace std;
Prescription::Prescription()
    : prescriptionId(0), appointmentId(0), patientId(0), doctorId(0) {
}
Prescription::Prescription(int pid, int aid, int patId, int docId,
    const string& d, const string& med,
    const string& n)
    : prescriptionId(pid), appointmentId(aid),
    patientId(patId), doctorId(docId),
    date(d), medicines(med), notes(n) {
}
int Prescription::getPrescriptionId() const { 
    return prescriptionId;
}
int Prescription::getAppointmentId() const {
    return appointmentId; 
}
int Prescription::getPatientId() const { 
    return patientId;
}
int Prescription::getDoctorId() const { 
    return doctorId;
}
const string& Prescription::getDate() const { 
    return date;
}
const string& Prescription::getMedicines() const {
    return medicines;
}
const string& Prescription::getNotes() const {
    return notes;
}
int Prescription::getId() const { 
    return prescriptionId;
}