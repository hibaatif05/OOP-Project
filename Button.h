#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include <string>
#include "Theme.h"

class Button {
public:
    enum class Style { PRIMARY, DANGER, SUCCESS, GHOST, WARNING };

private:
    sf::RectangleShape shape;
    sf::RectangleShape shadow;      
    sf::RectangleShape accentStrip;
    sf::Text label;
    sf::Color normalColor;
    sf::Color hoverColor;
    bool hovered = false;
    bool enabled = true;
    Style currentStyle;

public:
    Button();
    Button(const sf::Font& font, const std::string& text,
        sf::Vector2f pos, sf::Vector2f size,
        Style style = Style::PRIMARY);

    void setPosition(sf::Vector2f pos);
    void setSize(sf::Vector2f size);
    void setText(const std::string& text, const sf::Font& font);
    void setEnabled(bool e);

    // Returns true if this event caused a click on this button
    bool handleEvent(const sf::Event& e, const sf::RenderWindow& win);
    void draw(sf::RenderWindow& win);

    sf::FloatRect getBounds() const;
};

#endif

