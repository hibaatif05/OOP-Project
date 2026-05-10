#ifndef DESK_H
#define DESK_H


#include <SFML/Graphics.hpp>
#include <memory>
#include "Screen.h"
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

class Desk {
private:
    sf::RenderWindow window;
    sf::Font         font;
    sf::Clock        clock;
    sf::Texture bgTexture;
    sf::Sprite  bgSprite;

    // Shared data stores (loaded once on startup)
    Storage<Patient>      patients;
    Storage<Doctor>       doctors;
    Storage<Appointment>  appointments;
    Storage<Bill>         bills;
    Storage<Prescription> prescriptions;
    Admin                 admin;

    // Active logged-in IDs
    int loggedPatientId = -1;
    int loggedDoctorId = -1;

    // Current active screen
    std::unique_ptr<Screen> currentScreen;

    void loadData();
    void switchTo(ScreenID id);
    void buildLoginPatient();
    void buildLoginDoctor();
    void buildLoginAdmin();

public:
    Desk();
    void run();
};

#endif

