#ifndef REGISTERSCREEN_H
#define REGISTERSCREEN_H
#include "Screen.h"
#include "Button.h"
#include "TextBox.h"
#include "Storage.h"
#include "Patient.h"
#include "FileHandler.h"
#include <SFML/Graphics.hpp>

class RegisterScreen : public Screen {
private:
    sf::Font& font;
    Storage<Patient>& patients;
    sf::RectangleShape card;
    sf::RectangleShape topBar;
    sf::Text titleText;
    sf::Text msgText;
    sf::Text lName, lAge, lGender, lContact, lPassword, lBalance;
    TextBox  tbName, tbAge, tbGender, tbContact, tbPassword, tbBalance;

    Button   btnRegister;
    Button   btnBack;

    std::string message;
    bool        success = false;

    void tryRegister();

public:
    RegisterScreen(sf::Font& font, Storage<Patient>& patients);

    void handleEvent(const sf::Event& e, sf::RenderWindow& win) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;
};

#endif

