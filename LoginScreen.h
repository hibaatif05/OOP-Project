#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H
#include "Screen.h"
#include "Button.h"
#include "TextBox.h"
#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "FileHandler.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class LoginScreen : public Screen {
public:
    using SuccessCallback = std::function<void(int userId)>;

private:
    sf::Font& font;
    ScreenID            returnScreen;    
    ScreenID            successScreen;   

    sf::RectangleShape  card;
    sf::RectangleShape  topBar;
    sf::Text            titleText;
    sf::Text            idLabel;
    sf::Text            pwLabel;
    sf::Text            errorText;
    sf::Text            attemptsText;

    TextBox             tbId;
    TextBox             tbPassword;
    Button              btnLogin;
    Button              btnBack;

    // Data references for validation
    Storage<Patient>* patients = nullptr;
    Storage<Doctor>* doctors = nullptr;
    Admin* admin = nullptr;
    std::string role;           
    int failedAttempts = 0;
    bool locked = false;
    std::string errorMsg;
    SuccessCallback onSuccess;
    bool tryLogin();
    void logFailed(const std::string& enteredId);
public:
    // Constructor for Patient login
    LoginScreen(sf::Font& font, Storage<Patient>& patients,
        SuccessCallback cb);

    // Constructor for Doctor login
    LoginScreen(sf::Font& font, Storage<Doctor>& doctors,
        SuccessCallback cb);

    // Constructor for Admin login
    LoginScreen(sf::Font& font, Admin& admin,
        SuccessCallback cb);

    void setReturnScreen(ScreenID s) { returnScreen = s; }
    void setSuccessScreen(ScreenID s) { successScreen = s; }

    void handleEvent(const sf::Event& e, sf::RenderWindow& win) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;
};

#endif

