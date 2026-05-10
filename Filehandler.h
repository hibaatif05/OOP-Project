#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include <string>
using namespace std;
class FileHandler {
public:
    // Load all records from files
    static void loadPatients(Storage<Patient>& store);
    static void loadDoctors(Storage<Doctor>& store);
    static bool loadAdmin(Admin& admin);
    static void loadAppointments(Storage<Appointment>& store);
    static void loadBills(Storage<Bill>& store);
    static void loadPrescriptions(Storage<Prescription>& store);
    // Append a single new record
    static void appendPatient(const Patient& p);
    static void appendDoctor(const Doctor& d);
    static void appendAppointment(const Appointment& a);
    static void appendBill(const Bill& b);
    static void appendPrescription(const Prescription& p);
    // Rewrite entire file (used after update / delete)
    static void saveAllPatients(const Storage<Patient>& store);
    static void saveAllDoctors(const Storage<Doctor>& store);
    static void saveAllAppointments(const Storage<Appointment>& store);
    static void saveAllBills(const Storage<Bill>& store);
    static void saveAllPrescriptions(const Storage<Prescription>& store);
    // Security log
    static void appendSecurityLog(const string& timestamp,
        const string& role,
        const string& enteredId,
        const string& result);
    static void readSecurityLog();
    // Archive discharged patient
    static void archivePatient(const Patient& p,
        const Storage<Appointment>& appts,
        const Storage<Bill>& bills,
        const Storage<Prescription>& prescs);
};
#endif
