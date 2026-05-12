#ifndef ADMINSCREEN_H
#define ADMINSCREEN_H
#include "Screen.h"
#include "Button.h"
#include "TextBox.h"
#include "Theme.h"
#include "Admin.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
class AdminScreen : public Screen {
public:
    enum class View {
        MENU, ADD_DOCTOR, REMOVE_DOCTOR,
        ALL_PATIENTS, ALL_DOCTORS, ALL_APPOINTMENTS,
        UNPAID_BILLS, DISCHARGE, SECURITY_LOG, DAILY_REPORT
    };
private:
    sf::Font& font;
    Admin* admin;
    Storage<Patient>* patients;
    Storage<Doctor>* doctors;
    Storage<Appointment>* appointments;
    Storage<Bill>* bills;
    Storage<Prescription>* prescriptions;
    View currentView = View::MENU;
    sf::RectangleShape sidebar, topBar, contentArea, listBg;
    sf::Text           headerText, viewTitle, msgText;
    std::string        message;
    bool               msgSuccess = false;
    // Sidebar buttons
    Button btnAddDoc, btnRemoveDoc, btnPatients, btnDoctors,
        btnAppointments, btnUnpaid, btnDischarge,
        btnSecLog, btnReport, btnRegisterPat, btnLogout;
    std::vector<std::string> listRows;
    std::vector<float> colPositions;
    int scrollOffset = 0;

    sf::Text lName, lSpec, lContact, lPassword, lFee, lInput1;
    TextBox  tbName, tbSpec, tbContact, tbPassword, tbFee, tbInput1;
    Button   btnConfirm, btnBack2;

    void buildLayout();
    void setupAddDoctor();
    void setupRemoveDoctor();
    void setupAllPatients();
    void setupAllDoctors();
    void setupAllAppointments();
    void setupUnpaidBills();
    void setupDischarge();
    void setupSecurityLog();
    void setupDailyReport();
    void doAddDoctor();
    void doRemoveDoctor();
    void doDischarge();
    void drawList(sf::RenderWindow& win);
public:
    AdminScreen(sf::Font& font, Admin* admin,
        Storage<Patient>* patients,
        Storage<Doctor>* doctors,
        Storage<Appointment>* appointments,
        Storage<Bill>* bills,
        Storage<Prescription>* prescriptions);
    void handleEvent(const sf::Event& e, sf::RenderWindow& win) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;
};
#endif
