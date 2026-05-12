#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H
#include "Screen.h"
#include "Button.h"
#include <SFML/Graphics.hpp>

class MainMenuScreen : public Screen {
private:
    sf::Font& font;
    sf::Text title;
    sf::Text subtitle;
    sf::RectangleShape logo;
    sf::RectangleShape card;

    Button btnPatient;
    Button btnDoctor;
    Button btnAdmin;
    Button btnExit;

public:
    explicit MainMenuScreen(sf::Font& font);
    void handleEvent(const sf::Event& e, sf::RenderWindow& win) override;
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;
};

#endif
