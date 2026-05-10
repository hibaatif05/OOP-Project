#ifndef DOCTORSCREEN_H
#define DOCTORSCREEN_H
#include "Screen.h"
#include "Button.h"
#include "TextBox.h"
#include "Theme.h"
#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include "Prescription.h"
#include "Bill.h"
#include "Storage.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class DoctorScreen : public Screen {
public:
    enum class View { MENU, TODAY, MARK_COMPLETE, MARK_NOSHOW, PRESCRIPTION, HISTORY };

private:
    sf::Font& font;
    Doctor* doctor;
    Storage<Patient>* patients;
    Storage<Appointment>* appointments;
    Storage<Prescription>* prescriptions;
    Storage<Bill>* bills;

    View currentView = View::MENU;

    sf::RectangleShape sidebar, topBar, contentArea, listBg;
    sf::Text headerText, viewTitle, msgText;
    std::string message; bool msgSuccess = false;

    Button btnToday, btnComplete, btnNoShow, btnPrescription, btnHistory, btnLogout;

    std::vector<std::string> listRows;
    int scrollOffset = 0;

    sf::Text lInput1, lInput2, lInput3;
    TextBox  tbInput1, tbInput2, tbInput3;
    Button   btnConfirm, btnBack2;

    void buildLayout();
    void setupToday();
    void setupMarkAction(const std::string& action);
    void setupPrescription();
    void setupHistory();

    void doMarkComplete();
    void doMarkNoShow();
    void doWritePrescription();
    void doViewHistory();
    void drawList(sf::RenderWindow& win);

public:
    DoctorScreen(sf::Font& font, Doctor* doctor,
        Storage<Patient>* patients,
        Storage<Appointment>* appointments,
        Storage<Prescription>* prescriptions,
        Storage<Bill>* bills);

    void handleEvent(const sf::Event& e, sf::RenderWindow& win) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;
};

#endif

