#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H
#include <iostream>
#include <string>
using namespace std;
class Prescription {
private:
    int prescriptionId;
    int appointmentId;
    int patientId;
    int doctorId;
    string date;
    string medicines;
    string notes;
public:
    Prescription();
    Prescription(int prescId, int apptId, int patId, int docId,
        const string& date, const string& medicines,
        const string& notes);
    int getPrescriptionId() const;
    int getAppointmentId() const;
    int getPatientId() const;
    int getDoctorId() const;
    const string& getDate() const;
    const string& getMedicines() const;
    const string& getNotes() const;
    int getId() const;
};
#endif
