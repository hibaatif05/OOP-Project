#include "FileHandler.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;
static string fmtFloat(float v) {
    ostringstream ss;
    ss << fixed << setprecision(2) << v;
    return ss.str();
}
static void ensureFile(const string& path) {
    ifstream f(path);
    if (!f.good()) { ofstream o(path); } 
}
void FileHandler::loadPatients(Storage<Patient>& store) {
    ensureFile("patients.txt");
    ifstream f("patients.txt");
    string line;

    while (getline(f, line)) {
        if (line.empty()) continue;

        auto v = parseCSV(line, 7);
        if ((int)v.size() < 7) continue;

        store.add(Patient(stoi(v[0]), v[1], stoi(v[2]),
            v[3], v[4], v[5], stof(v[6])));
    }
}
void FileHandler::loadDoctors(Storage<Doctor>& store) {
    ensureFile("doctors.txt");
    ifstream f("doctors.txt");
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto v = parseCSV(line, 6);
        if ((int)v.size() < 6) continue;
        store.add(Doctor(stoi(v[0]), v[1], v[2], v[3], v[4], stof(v[5])));
    }
}
bool FileHandler::loadAdmin(Admin& admin) {
    ensureFile("admin.txt");
    ifstream f("admin.txt");
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto v = parseCSV(line, 3);
        if ((int)v.size() < 3) continue;
        admin = Admin(stoi(v[0]), v[1], v[2]);
        return true;
    }
    admin = Admin(1, "Admin", "admin123");
    ofstream o("admin.txt");
    o << "1,Admin,admin123\n";
    return true;
}
void FileHandler::loadAppointments(Storage<Appointment>& store) {
    ensureFile("appointments.txt");
    ifstream f("appointments.txt");
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto v = parseCSV(line, 6);
        if ((int)v.size() < 6) continue;
        store.add(Appointment(stoi(v[0]), stoi(v[1]),
            stoi(v[2]), v[3], v[4], v[5]));
    }
}
void FileHandler::loadBills(Storage<Bill>& store) {
    ensureFile("bills.txt");
    ifstream f("bills.txt");
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto v = parseCSV(line, 6);
        if ((int)v.size() < 6) continue;
        store.add(Bill(stoi(v[0]), stoi(v[1]),
            stoi(v[2]), stof(v[3]), v[4], v[5]));
    }
}
void FileHandler::loadPrescriptions(Storage<Prescription>& store) {
    ensureFile("prescriptions.txt");
    ifstream f("prescriptions.txt");
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto v = parseCSV(line, 7);
        if ((int)v.size() < 7) continue;
        store.add(Prescription(stoi(v[0]), stoi(v[1]),
            stoi(v[2]), stoi(v[3]),
            v[4], v[5], v[6]));
    }
}
void FileHandler::appendPatient(const Patient& p) {
    ofstream f("patients.txt", ios::app);
    f << p.getId() << "," << p.getName() << "," << p.getAge() << ","
        << p.getGender() << "," << p.getContact() << "," << p.getPassword()
        << "," << fmtFloat(p.getBalance()) << "\n";
}
void FileHandler::appendDoctor(const Doctor& d) {
    ofstream f("doctors.txt", ios::app);
    f << d.getId() << "," << d.getName() << "," << d.getSpecialization()
        << "," << d.getContact() << "," << d.getPassword()
        << "," << fmtFloat(d.getFee()) << "\n";
}
void FileHandler::appendAppointment(const Appointment& a) {
    ofstream f("appointments.txt", ios::app);
    f << a.getAppointmentId() << "," << a.getPatientId() << ","
        << a.getDoctorId() << "," << a.getDate() << ","
        << a.getTimeSlot() << "," << a.getStatus() << "\n";
}
void FileHandler::appendBill(const Bill& b) {
    ofstream f("bills.txt", ios::app);
    f << b.getBillId() << "," << b.getPatientId() << ","
        << b.getAppointmentId() << "," << fmtFloat(b.getAmount())
        << "," << b.getStatus() << "," << b.getDate() << "\n";
}
void FileHandler::appendPrescription(const Prescription& p) {
    ofstream f("prescriptions.txt", ios::app);
    f << p.getPrescriptionId() << "," << p.getAppointmentId() << ","
        << p.getPatientId() << "," << p.getDoctorId() << ","
        << p.getDate() << "," << p.getMedicines() << "," << p.getNotes() << "\n";
}
void FileHandler::saveAllPatients(const Storage<Patient>& store) {
    ofstream f("patients.txt");
    for (int i = 0; i < store.size(); ++i) {
        const Patient& p = store.getAt(i);
        f << p.getId() << "," << p.getName() << "," << p.getAge() << ","
            << p.getGender() << "," << p.getContact() << "," << p.getPassword()
            << "," << fmtFloat(p.getBalance()) << "\n";
    }
}
void FileHandler::saveAllDoctors(const Storage<Doctor>& store) {
    ofstream f("doctors.txt");
    for (int i = 0; i < store.size(); ++i) {
        const Doctor& d = store.getAt(i);
        f << d.getId() << "," << d.getName() << "," << d.getSpecialization()
            << "," << d.getContact() << "," << d.getPassword()
            << "," << fmtFloat(d.getFee()) << "\n";
    }
}
void FileHandler::saveAllAppointments(const Storage<Appointment>& store) {
    ofstream f("appointments.txt");
    for (int i = 0; i < store.size(); ++i) {
        const Appointment& a = store.getAt(i);
        f << a.getAppointmentId() << "," << a.getPatientId() << ","
            << a.getDoctorId() << "," << a.getDate() << ","
            << a.getTimeSlot() << "," << a.getStatus() << "\n";
    }
}
void FileHandler::saveAllBills(const Storage<Bill>& store) {
    ofstream f("bills.txt");
    for (int i = 0; i < store.size(); ++i) {
        const Bill& b = store.getAt(i);
        f << b.getBillId() << "," << b.getPatientId() << ","
            << b.getAppointmentId() << "," << fmtFloat(b.getAmount())
            << "," << b.getStatus() << "," << b.getDate() << "\n";
    }
}
void FileHandler::saveAllPrescriptions(const Storage<Prescription>& store) {
    ofstream f("prescriptions.txt");
    for (int i = 0; i < store.size(); ++i) {
        const Prescription& p = store.getAt(i);
        f << p.getPrescriptionId() << "," << p.getAppointmentId() << ","
            << p.getPatientId() << "," << p.getDoctorId() << ","
            << p.getDate() << "," << p.getMedicines() << "," << p.getNotes() << "\n";
    }
}
void FileHandler::appendSecurityLog(const string& ts, const string& role,
    const string& eid, const string& result) {
    ofstream f("security_log.txt", ios::app);
    f << ts << "," << role << "," << eid << "," << result << "\n";
}
void FileHandler::readSecurityLog() {
    ifstream f("security_log.txt");
    if (!f.good()) {
        cout << "No security events logged.\n";
        return;
    }
    string line;
    bool any = false;
    while (getline(f, line)) {
        if (!line.empty()) {
            cout << line << "\n";
            any = true;
        }
    }
    if (!any) cout << "No security events logged.\n";
}
void FileHandler::archivePatient(const Patient& p,
    const Storage<Appointment>& appts,
    const Storage<Bill>& bills,
    const Storage<Prescription>& prescs) {
    ofstream f("discharged.txt", ios::app);
    f << p.getId() << "," << p.getName() << "," << p.getAge() << ","
        << p.getGender() << "," << p.getContact() << "," << p.getPassword()
        << "," << fmtFloat(p.getBalance()) << "\n";
    for (int i = 0; i < appts.size(); ++i) {
        const Appointment& a = appts.getAt(i);
        if (a.getPatientId() != p.getId()) continue;
        f << a.getAppointmentId() << "," << a.getPatientId() << ","
            << a.getDoctorId() << "," << a.getDate() << ","
            << a.getTimeSlot() << "," << a.getStatus() << "\n";
    }
    for (int i = 0; i < bills.size(); ++i) {
        const Bill& b = bills.getAt(i);
        if (b.getPatientId() != p.getId()) continue;
        f << b.getBillId() << "," << b.getPatientId() << ","
            << b.getAppointmentId() << "," << fmtFloat(b.getAmount())
            << "," << b.getStatus() << "," << b.getDate() << "\n";
    }
    for (int i = 0; i < prescs.size(); ++i) {
        const Prescription& pr = prescs.getAt(i);
        if (pr.getPatientId() != p.getId()) continue;
        f << pr.getPrescriptionId() << "," << pr.getAppointmentId() << ","
            << pr.getPatientId() << "," << pr.getDoctorId() << ","
            << pr.getDate() << "," << pr.getMedicines() << "," << pr.getNotes() << "\n";
    }
}