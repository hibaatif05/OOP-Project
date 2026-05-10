#include "Desk.h"
#include "FileHandler.h"
#include "MainMenuScreen.h"
#include "LoginScreen.h"
#include "RegisterScreen.h"
#include "PatientScreen.h"
#include "DoctorScreen.h"
#include "AdminScreen.h"
#include <iostream>

Desk::Desk() {
    // Create window
    window.create(sf::VideoMode(Theme::WIN_W, Theme::WIN_H),
        "MediCore Hospital Management System",
        sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    // Load font
    if (!font.loadFromFile("assets/font.ttf")) {
        // Fallback: try common system fonts on Windows
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            if (!font.loadFromFile("C:/Windows/Fonts/calibri.ttf")) {
                std::cout << "Could not load font.\n";
                window.close();
                return;
            }
        }
    }
    if (bgTexture.loadFromFile("assets/background.png")) {
        bgSprite.setTexture(bgTexture);
        bgSprite.setScale(
            (float)Theme::WIN_W / bgTexture.getSize().x,
            (float)Theme::WIN_H / bgTexture.getSize().y
        );
    }

    loadData();
    // Start on main menu
    currentScreen = std::make_unique<MainMenuScreen>(font);
}

void Desk::loadData() {
    FileHandler::loadPatients(patients);
    FileHandler::loadDoctors(doctors);
    FileHandler::loadAdmin(admin);
    FileHandler::loadAppointments(appointments);
    FileHandler::loadBills(bills);
    FileHandler::loadPrescriptions(prescriptions);
}

// Screen switching 

void Desk::buildLoginPatient() {
    auto* s = new LoginScreen(font, patients, [this](int userId) {
        loggedPatientId = userId;
        });
    s->setReturnScreen(ScreenID::MAIN_MENU);
    s->setSuccessScreen(ScreenID::PATIENT_MENU);
    currentScreen.reset(s);
}

void Desk::buildLoginDoctor() {
    auto* s = new LoginScreen(font, doctors, [this](int userId) {
        loggedDoctorId = userId;
        });
    s->setReturnScreen(ScreenID::MAIN_MENU);
    s->setSuccessScreen(ScreenID::DOCTOR_MENU);
    currentScreen.reset(s);
}

void Desk::buildLoginAdmin() {
    auto* s = new LoginScreen(font, admin, [this](int /*userId*/) {});
    s->setReturnScreen(ScreenID::MAIN_MENU);
    s->setSuccessScreen(ScreenID::ADMIN_MENU);
    currentScreen.reset(s);
}

void Desk::switchTo(ScreenID id) {
    switch (id) {
    case ScreenID::MAIN_MENU:
        currentScreen = std::make_unique<MainMenuScreen>(font);
        break;

    case ScreenID::LOGIN_PATIENT:
        buildLoginPatient();
        break;

    case ScreenID::LOGIN_DOCTOR:
        buildLoginDoctor();
        break;

    case ScreenID::LOGIN_ADMIN:
        buildLoginAdmin();
        break;

    case ScreenID::REGISTER_PATIENT:
        currentScreen = std::make_unique<RegisterScreen>(font, patients);
        break;

    case ScreenID::PATIENT_MENU: {
        Patient* p = patients.findById(loggedPatientId);
        if (p) {
            currentScreen = std::make_unique<PatientScreen>(
                font, p, &patients, &doctors,
                &appointments, &bills, &prescriptions);
        }
        else {
            currentScreen = std::make_unique<MainMenuScreen>(font);
        }
        break;
    }

    case ScreenID::DOCTOR_MENU: {
        Doctor* d = doctors.findById(loggedDoctorId);
        if (d) {
            currentScreen = std::make_unique<DoctorScreen>(
                font, d, &patients, &appointments,
                &prescriptions, &bills);
        }
        else {
            currentScreen = std::make_unique<MainMenuScreen>(font);
        }
        break;
    }

    case ScreenID::ADMIN_MENU:
        currentScreen = std::make_unique<AdminScreen>(
            font, &admin, &patients, &doctors,
            &appointments, &bills, &prescriptions);
        break;

    default:
        currentScreen = std::make_unique<MainMenuScreen>(font);
        break;
    }
}

void Desk::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        //Events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (currentScreen) currentScreen->handleEvent(event, window);
        }

        //Update
        if (currentScreen) currentScreen->update(dt);

        //Screen transition
        if (currentScreen) {
            ScreenID next = currentScreen->getNextScreen();
            if (next != ScreenID::NONE) {
                currentScreen->resetNext();
                switchTo(next);
            }
        }

        //Draw
        window.clear(Theme::BG);
        window.draw(bgSprite);
        if (currentScreen) currentScreen->draw(window);
        window.display();
    }
}
