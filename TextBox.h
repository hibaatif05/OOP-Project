#ifndef TEXTBOX_H
#define TEXTBOX_H
#include <SFML/Graphics.hpp>
#include <string>
#include "Theme.h"
class TextBox {
private:
    sf::RectangleShape shape;
    sf::RectangleShape cursor;
    sf::Text displayText;
    sf::Text placeholderText;
    std::string value;
    bool active = false;
    bool password = false;  
    float cursorTimer = 0.f;
    bool cursorVis = true;
    void refreshDisplay();
public:
    TextBox();
    TextBox(const sf::Font& font, const std::string& placeholder,
        sf::Vector2f pos, sf::Vector2f size, bool isPassword = false);

    void setPosition(sf::Vector2f pos);
    void setActive(bool a);
    bool isActive() const;

    const std::string& getValue() const;
    void setValue(const std::string& v);
    void clear();
    bool handleEvent(const sf::Event& e, const sf::RenderWindow& win);
    void update(float dt);  
    void draw(sf::RenderWindow& win);
};
#endif

