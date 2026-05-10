#ifndef SCREEN_H
#define SCREEN_H
#include <SFML/Graphics.hpp>
enum class ScreenID {
    NONE = -1,
    MAIN_MENU,
    LOGIN_PATIENT,
    LOGIN_DOCTOR,
    LOGIN_ADMIN,
    REGISTER_PATIENT,
    PATIENT_MENU,
    DOCTOR_MENU,
    ADMIN_MENU
};

class Screen {
protected:
    ScreenID nextScreen = ScreenID::NONE;

public:
    virtual ~Screen() = default;
    virtual void handleEvent(const sf::Event& e, sf::RenderWindow& win) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& win) = 0;
    ScreenID getNextScreen() const { return nextScreen; }
    void     resetNext() { nextScreen = ScreenID::NONE; }
};

#endif

